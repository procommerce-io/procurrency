// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0,      uint256("0x0000040154f262df86de1eb539d4bb05ebd5aa8448bdbff066c614e705b5af26") )
		( 114, uint256("0x00000004bfa305e9add8227c9af533c866bd76fb6900bf7f257f401faac6c27e") )
		( 2149, uint256("0x0000000188ffffd28ee8c839e5d494d614ef5c2d6694519b692ddb7c3581a8a9") )
		( 4148, uint256("0x46f45a86573c065020b7aec78fad29416381d79f2cbdff472168473670e9c91e") )
		( 5151, uint256("0xf138238d8b93e31507e45a6bcf08ed58468db2ebcbe1ce5efc0fb0364bcfa974") )
		( 7289, uint256("0xf1b971b083ca8fed18fb2be5898f4bcc6e3a4de457a20f5fe4d10435e8a1dbaf") )
		( 30842, uint256("0x580e484cf41920aa1c2deb8f9ad15073f8445386d9bae4d6f6e3d94251ae88cf") )
		( 30853, uint256("0xb0c151de9e704764522b67703ec59cf07e57889c5e2752cb86b0a7fa2b120e88") )
		( 30856, uint256("0xf43a88cf9abe1eda8d3f74971a13e0fca77b18bb39a9ad84cda2b846ccbbef4f") )
		( 43205, uint256("0x167ac213aa70bbe9306b263f3dd9dce3590978ef6cd3f46372db141d573b8cd5") )
		( 60966, uint256("0xaa3903677bd294278b20b75ba25ae042af7fdc0bdedd248b4613f27e1d44ea8d") )
		( 61851, uint256("0x883b08ff0c31145c04733078f023e513fe67e7fa9b6f3fc948e61f6c01dd5cf4") )
		//( 3, uint256("0x") )
		//( 3, uint256("0x") )
		//( 3, uint256("0x") )
		
    ;

    // TestNet has no checkpoints
    MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end())
            return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    CBlockThinIndex* GetLastCheckpoint(const std::map<uint256, CBlockThinIndex*>& mapBlockThinIndex)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockThinIndex*>::const_iterator t = mapBlockThinIndex.find(hash);
            if (t != mapBlockThinIndex.end())
                return t->second;
        }
        return NULL;
    }


    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Automatically select a suitable sync-checkpoint - Thin mode
    const CBlockThinIndex* AutoSelectSyncThinCheckpoint()
    {
        const CBlockThinIndex *pindex = pindexBestHeader;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        if(nNodeMode == NT_FULL)
        {
            const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

            if (nHeight <= pindexSync->nHeight)
                return false;
        }
        else {
            const CBlockThinIndex *pindexSync = AutoSelectSyncThinCheckpoint();

            if (nHeight <= pindexSync->nHeight)
                return false;
        }
        return true;
    }
}
