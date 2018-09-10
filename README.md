# ProCurrency Codebase Development

===========================================

### Coin Specs
<table>
<tr><td>Algo</td><td>Sha256 (Pure POS)</td></tr>
<tr><td>Ticker</td><td>PROC</td></tr>
<tr><td>Block Time</td><td>90 Seconds</td></tr>
<tr><td>Coin Age</td><td>6 Hrs</td></tr>
<tr><td>Difficulty Retargeting</td><td>Every Block</td></tr>
<tr><td>Max Coin Supply</td><td>75B PROC</td></tr>
<tr><td>Premine</td><td>15B PROC</td></tr>

<tr><td>RPC Port</td><td> <s>35960</s> now 45960</td></tr>
<tr><td>P2P Port</td><td> <s>35950</s> now 45950</td></tr>
</table>

### NOTE:
ProCurrency need secp256k1 with Module Recovery Enabled

### libsecp256k1 Build steps

libsecp256k1 is built using autotools:

    $ ./autogen.sh
    $ ./configure --enable-module-recovery
    $ make
	
	To Build or Run ProCurrency-qt for Linux
    $ sudo make install
