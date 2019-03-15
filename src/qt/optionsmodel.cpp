// Copyright (c) 2011-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "optionsmodel.h"
#include "bitcoinunits.h"


#include "init.h"
#include "guiutil.h"
#include "ringsig.h"

#ifdef ENABLE_WALLET
#include "wallet.h"
#include "walletdb.h"
#endif

#include <QSettings>

OptionsModel::OptionsModel(QObject *parent) :
    QAbstractListModel(parent)
{
    Init();
}

bool static ApplyProxySettings()
{
    QSettings settings;
    CService addrProxy(settings.value("addrProxy", "127.0.0.1:9050").toString().toStdString());
    
    if (!settings.value("fUseProxy", false).toBool()) {
        addrProxy = CService();
        return false;
    }
    if (!addrProxy.IsValid())
        return false;
    if (!IsLimited(NET_IPV4))
    SetProxy(NET_IPV4, addrProxy);
    if (!IsLimited(NET_IPV6))
        SetProxy(NET_IPV6, addrProxy);
    SetNameProxy(addrProxy);
    return true;
}

void OptionsModel::Init()
{
    QSettings settings;

    // These are Qt-only settings:
	
	// Window
    fMinimizeToTray = settings.value("fMinimizeToTray", false).toBool();
    fMinimizeOnClose = settings.value("fMinimizeOnClose", false).toBool();
	
	// Display
    nDisplayUnit = settings.value("nDisplayUnit", BitcoinUnits::PROC).toInt();
    bDisplayAddresses = settings.value("bDisplayAddresses", false).toBool();
	language = settings.value("language", "").toString();
	
	// Main
    nTransactionFee = settings.value("nTransactionFee").toLongLong();
    nReserveBalance = settings.value("nReserveBalance").toLongLong();
    nRowsPerPage = settings.value("nRowsPerPage", 20).toInt();
    notifications = settings.value("notifications", "*").toStringList();
    visibleTransactions = settings.value("visibleTransactions", "*").toStringList();
    fAutoRingSize = settings.value("fAutoRingSize", false).toBool();
    fAutoRedeemToken = settings.value("fAutoRedeemToken", false).toBool();
    nMinRingSize = settings.value("nMinRingSize", MIN_RING_SIZE).toInt();
    nMaxRingSize = settings.value("nMaxRingSize", MAX_RING_SIZE).toInt();

	
    // These are shared with core Bitcoin; we want
    // command-line options to override the GUI settings.
	//
    // If setting doesn't exist create it with defaults.
    //
    // If SoftSetArg() or SoftSetBoolArg() return false we were overridden
    // by command-line and show this in the UI.
	
	// Network
    if (settings.contains("fUseUPnP"))
        SoftSetBoolArg("-upnp", settings.value("fUseUPnP").toBool());
    if (settings.contains("addrProxy") && settings.value("fUseProxy").toBool())
        SoftSetArg("-proxy", settings.value("addrProxy").toString().toStdString());
    if (settings.contains("nSocksVersion") && settings.value("fUseProxy").toBool())
        SoftSetArg("-socks", settings.value("nSocksVersion").toString().toStdString());
	
	// Display
    if (!language.isEmpty())
        SoftSetArg("-lang", language.toStdString());
	
	// Main
	if (settings.contains("detachDB"))
        SoftSetBoolArg("-detachdb", settings.value("detachDB").toBool());
    if (settings.contains("fStaking"))
        SoftSetBoolArg("-staking", settings.value("fStaking").toBool());
    if (settings.contains("nMinStakeInterval"))
        SoftSetArg("-minstakeinterval", settings.value("nMinStakeInterval").toString().toStdString());
    if (settings.contains("fSecMsgEnabled"))
        SoftSetBoolArg("-nosmsg", !settings.value("fSecMsgEnabled").toBool());
    if (settings.contains("fThinMode"))
        SoftSetBoolArg("-thinmode", settings.value("fThinMode").toBool());
    if (settings.contains("fThinFullIndex"))
        SoftSetBoolArg("-thinfullindex", settings.value("fThinFullIndex").toBool());
    if (settings.contains("nThinIndexWindow"))
        SoftSetArg("-thinindexmax", settings.value("nThinIndexWindow").toString().toStdString());
	if (!settings.contains("fCheckForUpdates"))
        settings.setValue("fCheckForUpdates", DEFAULT_CHECK_FOR_UPDATES);
    fCheckForUpdates = settings.value("fCheckForUpdates").toBool();
	
}

void OptionsModel::Reset()
{
    QSettings settings;

    // Remove all entries from our QSettings object
    settings.clear();

    // default setting for OptionsModel::StartAtStartup - disabled
    if (GUIUtil::GetStartOnSystemStartup())
        GUIUtil::SetStartOnSystemStartup(false);
}

int OptionsModel::rowCount(const QModelIndex & parent) const
{
    return OptionIDRowCount;
}

// read QSettings values and return them
QVariant OptionsModel::data(const QModelIndex & index, int role) const
{
    if(role == Qt::EditRole)
    {
        QSettings settings;
        switch(index.row())
        {
        case StartAtStartup:
            return GUIUtil::GetStartOnSystemStartup();
        case MinimizeToTray:
            return fMinimizeToTray;
        case MapPortUPnP:
            return settings.value("fUseUPnP", GetBoolArg("-upnp", true));
        case MinimizeOnClose:
            return fMinimizeOnClose;
        case ProxyUse:
            return settings.value("fUseProxy", false);
        case ProxyIP: {
            proxyType proxy;
            if (GetProxy(NET_IPV4, proxy))
                return QString::fromStdString(proxy.ToStringIP());
            else
                return "";
        }
        case ProxyPort: {
            proxyType proxy;
            if (GetProxy(NET_IPV4, proxy))
                return QVariant(proxy.GetPort());
        }
            break;

        case ProxySocksVersion:
            return settings.value("nSocksVersion", 5);
#ifdef ENABLE_WALLET			
        case Fee:
            return (qint64) nTransactionFee;
        case ReserveBalance:
            return (qint64) nReserveBalance;
#endif			
        case DisplayUnit:
            return nDisplayUnit;
        case DisplayAddresses:
            return bDisplayAddresses;
        case DetachDatabases:
            return bitdb.GetDetach();
        case Language:
            return settings.value("language", "");
        case RowsPerPage:
            return nRowsPerPage;
        case AutoRingSize:
            return fAutoRingSize;
        case AutoRedeemToken:
            return fAutoRedeemToken;
        case MinRingSize:
            return nMinRingSize;
        case MaxRingSize:
            return nMaxRingSize;
        case Staking:
            return settings.value("fStaking", GetBoolArg("-staking", true)).toBool();
        case MinStakeInterval:
            return nMinStakeInterval;
        case SecureMessaging:
            return fSecMsgEnabled;
        case ThinMode:
            return settings.value("fThinMode",      GetBoolArg("-thinmode",      false)).toBool();
        case ThinFullIndex:
            return settings.value("fThinFullIndex", GetBoolArg("-thinfullindex", false)).toBool();
        case ThinIndexWindow:
            return settings.value("ThinIndexWindow", (qint64) GetArg("-thinindexwindow", 4096)).toInt();
        case Notifications:
            return notifications;
        case VisibleTransactions:
            return visibleTransactions;
		case CheckForUpdates:
            return settings.value("fCheckForUpdates");
        default:
            return QVariant();	
        }
    }

    return QVariant();
}

QString OptionsModel::optionIDName(int row)
{
    switch(row)
    {
    case Fee: return "Fee";
    case ReserveBalance: return "ReserveBalance";
    case StartAtStartup: return "StartAtStartup";
    case DetachDatabases: return "DetachDatabases";
    case Staking: return "Staking";
    case MinStakeInterval: return "MinStakeInterval";
    case SecureMessaging: return "SecureMessaging";
    case ThinMode: return "ThinMode";
    case ThinFullIndex: return "ThinFullIndex";
    case ThinIndexWindow: return "ThinIndexWindow";
    case AutoRingSize: return "AutoRingSize";
    case AutoRedeemToken: return "AutoRedeemToken";
    case MinRingSize: return "MinRingSize";
    case MaxRingSize: return "MaxRingSize";
    case MapPortUPnP: return "MapPortUPnP";
    case ProxyUse: return "ProxyUse";
    case ProxyIP: return "ProxyIP";
    case ProxyPort: return "ProxyPort";
    case ProxySocksVersion: return "ProxySocksVersion";
    case MinimizeToTray: return "MinimizeToTray";
    case MinimizeOnClose: return "MinimizeOnClose";
    case Language: return "Language";
    case DisplayUnit: return "DisplayUnit";
    case DisplayAddresses: return "DisplayAddresses";
    case RowsPerPage: return "RowsPerPage";
    case Notifications: return "Notifications";
    case VisibleTransactions: return "VisibleTransactions";
    }

    return "";
}

int OptionsModel::optionNameID(QString name)
{
    for(int i=0;i<OptionIDRowCount;i++)
        if(optionIDName(i) == name)
            return i;

    return -1;
}

// write QSettings values
bool OptionsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    bool successful = true; /* set to false on parse error */
    if(role == Qt::EditRole)
    {
        QSettings settings;
        switch(index.row())
        {
        case StartAtStartup:
            successful = GUIUtil::SetStartOnSystemStartup(value.toBool());
            break;
        case MinimizeToTray:
            fMinimizeToTray = value.toBool();
            settings.setValue("fMinimizeToTray", fMinimizeToTray);
            break;
        case MapPortUPnP:
            fUseUPnP = value.toBool();
            settings.setValue("fUseUPnP", value.toBool());
            MapPort(value.toBool());
            break;
        case MinimizeOnClose:
            fMinimizeOnClose = value.toBool();
            settings.setValue("fMinimizeOnClose", fMinimizeOnClose);
            break;
        case ProxyUse:
            settings.setValue("fUseProxy", value.toBool());
            ApplyProxySettings();
            break;
        case ProxyIP: {
            proxyType proxy;
            proxy = CService("127.0.0.1", 9050);
            GetProxy(NET_IPV4, proxy);

            CNetAddr addr(value.toString().toStdString());
            proxy.SetIP(addr);
            settings.setValue("addrProxy", proxy.ToStringIPPort().c_str());
            successful = ApplyProxySettings();
        }
        break;
        case ProxyPort: {
            proxyType proxy;
            proxy = CService("127.0.0.1", 9050);
            GetProxy(NET_IPV4, proxy);

            proxy.SetPort(value.toInt());
            settings.setValue("addrProxy", proxy.ToStringIPPort().c_str());
            successful = ApplyProxySettings();
        }
        break;
        case Fee:
            nTransactionFee = value.toLongLong();
            settings.setValue("nTransactionFee", (qint64) nTransactionFee);
            emit transactionFeeChanged(nTransactionFee);
            break;
        case ReserveBalance:
            nReserveBalance = value.toLongLong();
            settings.setValue("nReserveBalance", (qint64) nReserveBalance);
            emit reserveBalanceChanged(nReserveBalance);
            break;
        case DisplayUnit:
            nDisplayUnit = value.toInt();
            settings.setValue("nDisplayUnit", nDisplayUnit);
            emit displayUnitChanged(nDisplayUnit);
            break;
        case DisplayAddresses:
            bDisplayAddresses = value.toBool();
            settings.setValue("bDisplayAddresses", bDisplayAddresses);
            emit displayUnitChanged(settings.value("nDisplayUnit", BitcoinUnits::PROC).toInt());
            break;
        case DetachDatabases: {
            bool fDetachDB = value.toBool();
            bitdb.SetDetach(fDetachDB);
            settings.setValue("detachDB", fDetachDB);
            }
            break;
        case Language:
            settings.setValue("language", value);
            break;
        case RowsPerPage: {
            nRowsPerPage = value.toInt();
            settings.setValue("nRowsPerPage", nRowsPerPage);
            emit rowsPerPageChanged(nRowsPerPage);
            }
            break;
        case Notifications: {
            notifications = value.toStringList();
            settings.setValue("notifications", notifications);
            }
            break;
        case VisibleTransactions: {
            visibleTransactions = value.toStringList();
            settings.setValue("visibleTransactions", visibleTransactions);
            emit visibleTransactionsChanged(visibleTransactions);
            }
            break;
        case AutoRingSize: {
            fAutoRingSize = value.toBool();
            settings.setValue("fAutoRingSize", fAutoRingSize);
            }
            break;
        case AutoRedeemToken: {
            fAutoRedeemToken = value.toBool();
            settings.setValue("fAutoRedeemToken", fAutoRedeemToken);
            }
            break;
        case MinRingSize: {
            nMinRingSize = value.toInt();
            settings.setValue("nMinRingSize", nMinRingSize);
            }
            break;
        case MaxRingSize: {
            nMaxRingSize = value.toInt();
            settings.setValue("nMaxRingSize", nMaxRingSize);
            }
            break;
        case Staking:
            settings.setValue("fStaking", value.toBool());
            break;
        case MinStakeInterval:
            nMinStakeInterval = value.toInt();
            settings.setValue("nMinStakeInterval", nMinStakeInterval);
            break;
        case ThinMode:
            settings.setValue("fThinMode", value.toBool());
            break;
        case ThinFullIndex:
            settings.setValue("fThinFullIndex", value.toBool());
            break;
        case ThinIndexWindow:
            settings.setValue("fThinIndexWindow", value.toInt());
            break;
        case SecureMessaging: {
            if(value.toBool())
            {
                if(!fSecMsgEnabled)
                    SecureMsgEnable();
            }
            else
                SecureMsgDisable();

            settings.setValue("fSecMsgEnabled", fSecMsgEnabled);
            }
            break;
		case CheckForUpdates:
            if (settings.value("fCheckForUpdates") != value) {
                settings.setValue("fCheckForUpdates", value);
                fCheckForUpdates = value.toBool();
            }
            break;	
        default:
            break;
        }
    }
    emit dataChanged(index, index);

    return successful;
}

qint64 OptionsModel::getTransactionFee()
{
    return nTransactionFee;
}

qint64 OptionsModel::getReserveBalance()
{
    return nReserveBalance;
}

bool OptionsModel::getMinimizeToTray()
{
    return fMinimizeToTray;
}

bool OptionsModel::getMinimizeOnClose()
{
    return fMinimizeOnClose;
}

int OptionsModel::getDisplayUnit()
{
    return nDisplayUnit;
}

bool OptionsModel::getDisplayAddresses()
{
    return bDisplayAddresses;
}

/*void OptionsModel::setRestartRequired(bool fRequired)
{
    QSettings settings;
    return settings.setValue("fRestartRequired", fRequired);
}

bool OptionsModel::isRestartRequired()
{
    QSettings settings;
    return settings.value("fRestartRequired", false).toBool();
}*/

int OptionsModel::getRowsPerPage() { return nRowsPerPage; }
QStringList OptionsModel::getNotifications() { return notifications; }
QStringList OptionsModel::getVisibleTransactions() { return visibleTransactions; }
bool OptionsModel::getAutoRingSize() { return fAutoRingSize; }
bool OptionsModel::getAutoRedeemToken() { return fAutoRedeemToken; }
int OptionsModel::getMinRingSize() { return nMinRingSize; }
int OptionsModel::getMaxRingSize() { return nMaxRingSize; }
