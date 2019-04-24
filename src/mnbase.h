// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Copyright (c) 2014-2015 The ProCurrency developers
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#ifndef PROCURRENCY_MNBASE_H
#define PROCURRENCY_MNBASE_H

#include "main.h"



/** Masternodes Payments Start Time **/
#define START_MASTERNODE_PAYMENTS_TESTNET 1532602569
#define START_MASTERNODE_PAYMENTS 1532602569
/*** Darksend Coolateral And Pool Max **/
static const int64_t DARKSEND_COLLATERAL = (0.02*COIN);
static const int64_t DARKSEND_POOL_MAX = (999999.99*COIN);

/** InstantX Signatues **/
#define INSTANTX_SIGNATURES_REQUIRED           10
#define INSTANTX_SIGNATURES_TOTAL              15

/*** Masternodes Collateral ***/
inline int64_t GetMNCollateral(int nHeight) { return 1000000; } ///Standar MN Collateral set to 1M PROC




#endif


