// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

// miner's coin base reward
int64_t CChainParams::GetProofOfWorkReward(int nHeight, int64_t nFees) const
{
    int64_t nSubsidy = 0 * COIN;
 
    if(nHeight == 1)
    {
        nSubsidy = 15000000000 * COIN;   // 15B
    }
        else
    {
        nSubsidy = 0 * COIN;
    }
            
    if (fDebug && GetBoolArg("-printcreation"))
        LogPrintf("GetProofOfWorkReward() : create=%s nSubsidy=%d\n", FormatMoney(nSubsidy).c_str(), nSubsidy);
    
    return nSubsidy + nFees;
}

//static const int YEARLY_BLOCKCOUNT =  525600; // 1440 * 365 //del
static const int YEARLY_BLOCKCOUNT =  899790; //
// miner's coin stake reward based on coin age spent (coin-days)
int64_t CChainParams::GetProofOfStakeReward(int nHeight, int64_t nCoinAge, int64_t nFees) const
{
    int64_t nSubsidy = 5 * COIN;
	
	//if(nHeight < 100)
	if(nHeight < (YEARLY_BLOCKCOUNT))
    {
        nSubsidy = 15 * COIN;   // 
    }
	else
	{
		nSubsidy = 5 * COIN;
	}

    if (fDebug && GetBoolArg("-printcreation"))
        LogPrintf("GetProofOfStakeReward(): create=%s nCoinAge=%d\n", FormatMoney(nSubsidy).c_str(), nCoinAge);
    
    return nSubsidy + nFees;
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

// Convert the pnSeeds array into usable address objects.
static void convertSeeds(std::vector<CAddress> &vSeedsOut, const unsigned int *data, unsigned int count, int port)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int k = 0; k < count; ++k)
    {
        struct in_addr ip;
        unsigned int i = data[k], t;
        
        // -- convert to big endian
        t =   (i & 0x000000ff) << 24u
            | (i & 0x0000ff00) << 8u
            | (i & 0x00ff0000) >> 8u
            | (i & 0xff000000) >> 24u;
        
        memcpy(&ip, &t, sizeof(ip));
        
        CAddress addr(CService(ip, port));
        addr.nTime = GetTime()-GetRand(nOneWeek)-nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CBaseChainParams : public CChainParams {
public:
    CBaseChainParams() {
        const char* pszTimestamp = "March 1st 2017, ProCurrency Initially Launched its Blokchain";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, GENESIS_BLOCK_TIME, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = GENESIS_BLOCK_TIME;
        
        
    }
    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual const std::vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    std::vector<CAddress> vFixedSeeds;
};

class CMainParams : public CBaseChainParams {
public:
    CMainParams() {
        strNetworkID = "mainnet";
        
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x14;
        pchMessageStart[1] = 0xe5;
        pchMessageStart[2] = 0xa2;
        pchMessageStart[3] = 0xd3;
        
        vAlertPubKey = ParseHex("");
        
        nDefaultPort = 45950; //45950 / 35950
        nRPCPort = 45960;     //45960 / 35960
        nBIP44ID = 0x80000023;
        
        nLastPOWBlock = 5000;
        
        nFirstPosv2Block = 920; // POSv2 Start
        nFirstPosv3Block = 30000; // POSv3 Start
		nFirstPosv4Block = 440000; // POSv4 start

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20); // PoW starting difficulty = 0.0002441
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20); // PoS starting difficulty = 0.0002441
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 34);
		bnProofOfStakeLimitV3 = CBigNum(~uint256(0) >> 34);
		bnProofOfStakeLimitV4 = CBigNum(~uint256(0) >> 40); // ProtocolV4
        
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 163555;
        hashGenesisBlock = genesis.GetHash();
        
        assert(hashGenesisBlock == uint256("0x0000040154f262df86de1eb539d4bb05ebd5aa8448bdbff066c614e705b5af26"));
        assert(genesis.hashMerkleRoot == uint256("0x2c859a6fd90361e455f327235fb106546ea6ef1f5821462be3094c94ca80b13b"));
		
		
		
        vSeeds.push_back(CDNSSeedData("seed1",  "185.5.53.201"));
		vSeeds.push_back(CDNSSeedData("seed2",  "185.5.54.65"));
		vSeeds.push_back(CDNSSeedData("seed3",  "185.69.53.90"));
		vSeeds.push_back(CDNSSeedData("seed4",  "45.63.51.33"));
		vSeeds.push_back(CDNSSeedData("seed5",  "45.32.167.248"));
		vSeeds.push_back(CDNSSeedData("seed6",  "45.76.5.197"));
		vSeeds.push_back(CDNSSeedData("seed7",  "45.32.176.210"));
		vSeeds.push_back(CDNSSeedData("seed8",  "45.76.88.24"));
		vSeeds.push_back(CDNSSeedData("seed9",  "45.76.104.85"));
		
		vSeeds.push_back(CDNSSeedData("seed10",  "45.76.75.49"));
		vSeeds.push_back(CDNSSeedData("seed11",  "195.181.241.20"));
		vSeeds.push_back(CDNSSeedData("seed12",  "94.176.234.18"));
		vSeeds.push_back(CDNSSeedData("seed13",  "172.114.134.215"));
		//vSeeds.push_back(CDNSSeedData("seed12",  "94.176.232.135")); //del
		//
		/*
		vSeeds.push_back(CDNSSeedData("seed13",  "71.213.148.206"));
		vSeeds.push_back(CDNSSeedData("seed14",  "67.197.66.122"));
		vSeeds.push_back(CDNSSeedData("seed15",  "75.70.40.167"));
		vSeeds.push_back(CDNSSeedData("seed16",  "66.222.148.57"));
		vSeeds.push_back(CDNSSeedData("seed17",  "47.157.52.95"));
		*/
		
        
        base58Prefixes[PUBKEY_ADDRESS]      = list_of(55).convert_to_container<std::vector<unsigned char> >(); // ProCurrency address start with "P"
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(119).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]          = list_of(205).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[STEALTH_ADDRESS]     = list_of(125).convert_to_container<std::vector<unsigned char> >(); // s
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(93).convert_to_container<std::vector<unsigned char> >();         // e
        base58Prefixes[EXT_ACC_HASH]        = list_of(118).convert_to_container<std::vector<unsigned char> >();          // p
        base58Prefixes[EXT_PUBLIC_KEY_2]  = list_of(0xE6)(0x76)(0x34)(0x5A).convert_to_container<std::vector<unsigned char> >(); //
        base58Prefixes[EXT_SECRET_KEY_2]  = list_of(0xE6)(0x76)(0x28)(0x1D).convert_to_container<std::vector<unsigned char> >(); //

        //convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
        convertSeeds(vFixedSeeds, pnSeed, ARRAYLEN(pnSeed), nDefaultPort);
    }
    
    virtual Network NetworkID() const { return CChainParams::MAIN; }
};
static CMainParams mainParams;

//
// Testnet
//

class CTestNetParams : public CBaseChainParams {
public:
    CTestNetParams() {
        strNetworkID = "testnet";
        strDataDir = "testnet";
        
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xae;
        pchMessageStart[1] = 0xf4;
        pchMessageStart[2] = 0xe2;
        pchMessageStart[3] = 0x11;
        
        vAlertPubKey = ParseHex("04e20146b21f90537b95bfac738d1a7f8dc4c1dde0970c967c6d1a8afde077c07eb2fc8082bc4a79a7c97c369ecb9c76b414d1cdae93b8b8a8aa16400bff9d9e76");
        
        nDefaultPort = 45951; //45951 / 35951
        nRPCPort = 45961; //45961 / 35961
        nBIP44ID = 0x80000001;

        nLastPOWBlock = 1000;

        nFirstPosv2Block = 2000;
        nFirstPosv3Block = 3000;
        
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 16);

        /*genesis.nBits  = bnProofOfWorkLimit.GetCompact();*/
		genesis.nBits  = 0x1e0fffff; //504365055 Decimal Number
        genesis.nNonce = 163555;
        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x0000040154f262df86de1eb539d4bb05ebd5aa8448bdbff066c614e705b5af26"));
        
        base58Prefixes[PUBKEY_ADDRESS]      = list_of(127).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(196).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]          = list_of(202).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[STEALTH_ADDRESS]     = list_of(125).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >(); //tpub
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >(); //tprv
        base58Prefixes[EXT_KEY_HASH]        = list_of(92).convert_to_container<std::vector<unsigned char> >();         // e
        base58Prefixes[EXT_ACC_HASH]        = list_of(117).convert_to_container<std::vector<unsigned char> >();          // p
        base58Prefixes[EXT_PUBLIC_KEY_2]  = list_of(0xE6)(0x76)(0x34)(0x5A).convert_to_container<std::vector<unsigned char> >(); //
        base58Prefixes[EXT_SECRET_KEY_2]  = list_of(0xE6)(0x76)(0x28)(0x1D).convert_to_container<std::vector<unsigned char> >(); //
        
        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
        convertSeeds(vFixedSeeds, pnTestnetSeed, ARRAYLEN(pnTestnetSeed), nDefaultPort);
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strDataDir = "regtest";

        nFirstPosv2Block = -1;
        nFirstPosv3Block = -1;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1488375642;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 0;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 35952;
        
       //assert(hashGenesisBlock == uint256("0x0000040154f262df86de1eb539d4bb05ebd5aa8448bdbff066c614e705b5af26"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

const CChainParams &TestNetParams() {
    return testNetParams;
}

const CChainParams &MainNetParams() {
    return mainParams;
}

void SelectParams(CChainParams::Network network)
{
    switch (network)
    {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    };
};

bool SelectParamsFromCommandLine()
{
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest)
    {
        return false;
    };

    if (fRegTest)
    {
        SelectParams(CChainParams::REGTEST);
    } else
    if (fTestNet)
    {
        SelectParams(CChainParams::TESTNET);
    } else
    {
        SelectParams(CChainParams::MAIN);
    };
    
    return true;
}
