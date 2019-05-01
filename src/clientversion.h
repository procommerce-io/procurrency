// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 20017-2019 The ProCurrency Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PROCURRENCY_CLIENTVERSION_H
#define PROCURRENCY_CLIENTVERSION_H

//#include <string>

//
// client versioning
//

// These need to be macros, as version.cpp's and bitcoin-qt.rc's voodoo requires it
#define CLIENT_VERSION_MAJOR       1
#define CLIENT_VERSION_MINOR       39
#define CLIENT_VERSION_REVISION    2
#define CLIENT_VERSION_BUILD       0

// Converts the parameter X to a string after macro replacement on X has been performed.
// Don't merge these into one macro!
#define STRINGIZE(X) DO_STRINGIZE(X)
#define DO_STRINGIZE(X) #X


#endif // PROCURRENCY_CLIENTVERSION_H
