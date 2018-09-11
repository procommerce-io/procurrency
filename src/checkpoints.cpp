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
		( 5151, uint256("0xf138238d8b93e31507e45a6bcf08ed58468db2ebcbe1ce5efc0fb0364bcfa974") )
		( 30856, uint256("0xf43a88cf9abe1eda8d3f74971a13e0fca77b18bb39a9ad84cda2b846ccbbef4f") )
		( 43205, uint256("0x167ac213aa70bbe9306b263f3dd9dce3590978ef6cd3f46372db141d573b8cd5") )
		( 61851, uint256("0x883b08ff0c31145c04733078f023e513fe67e7fa9b6f3fc948e61f6c01dd5cf4") )
		( 156545, uint256("0x0c871e0f9a9c72e3191d09c40eeae4b4d0df52141b2331e392341d1c8d57c6ea") )
		( 193805, uint256("0xc7af5a3190199321e0bfbecc75c3af5da280ea9ecbd7e3d52ddf089dcd6a6e62") )
		( 252205, uint256("0x64d0ec28d37a774b7583ed4c789c47ac956bebee6699557b615776ac55780f80") )
		( 304210, uint256("0x0d47c2bdbede1c00976616ebd41ca656b465b63148173db20c869d420aa349bc") )
		( 404012, uint256("0xcd2ddfccb141da9e3403981eb86afd5161db80b4da9659494efef79687e347de") )
		//( xxx, uint256("0x") )
		//( xxx, uint256("0x") )
		//( xxx, uint256("0x") )
		
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
