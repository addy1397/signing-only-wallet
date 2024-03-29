#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <Hash.h>

using namespace std;

struct Input {
    string prev_tx_hash; // 64
    string prev_output_index; //8
    string script_len; // 57
    string electron_specific; // 8
    string xpub_key_type; // 2
    string serial_ver_bytes; // 8
    string depth; // 2
    string fingerprint; // 8
    string child_number; // 8
    string chain_code; // 64
    string pub_key; // 66
    string derivation; // 8
    string sequence; // 8
};

struct ScriptPubKey {
    string pub_key_hash; // 50
};

struct Output {
    string amount; // 16
    string script_len; // 2
    ScriptPubKey scriptPubKey; // 50
};

struct DerSignature {
    string der_prefex = "30";
    string len = "45";
    string r_marker = "02";
    string r_len; // 20 or 21
    string r_value; // 21 (big endian)
    string s_marker = "02";
    string s_len; // 20 or 21
    string s_value; // 21 (big endian)
};

class UnsignedTxn {
private:
    string electrum_partial_tx; // 8
    string separator; // 2
    string electrum_partial_tx_ver; // 2
    string network_ver; // 8
    string no_of_inputs; // 2
    vector<Input> input; // no_of_inputs
    string no_of_outputs; // 2
    vector<Output> output; // no_of_outputs
    string locktime; // 8
public:

    const string &getElectrumPartialTx() const {
        return electrum_partial_tx;
    }

    const vector<Input> &getInput() const {
        return input;
    }

    void setInput(const vector<Input> &input) {
        UnsignedTxn::input = input;
    }

    const vector<Output> &getOutput() const {
        return output;
    }

    void setOutput(const vector<Output> &output) {
        UnsignedTxn::output = output;
    }

    void setElectrumPartialTx(const string &electrumPartialTx) {
        electrum_partial_tx = electrumPartialTx;
    }

    const string &getSeparator() const {
        return separator;
    }

    void setSeparator(const string &separator) {
        UnsignedTxn::separator = separator;
    }

    const string &getElectrumPartialTxVer() const {
        return electrum_partial_tx_ver;
    }

    void setElectrumPartialTxVer(const string &electrumPartialTxVer) {
        electrum_partial_tx_ver = electrumPartialTxVer;
    }

    const string &getNetworkVer() const {
        return network_ver;
    }

    void setNetworkVer(const string &networkVer) {
        network_ver = networkVer;
    }

    const string &getLocktime() const {
        return locktime;
    }

    const string &getNoOfInputs() const {
        return no_of_inputs;
    }

    void setNoOfInputs(const string &noOfInputs) {
        no_of_inputs = noOfInputs;
    }

    const string &getNoOfOutputs() const {
        return no_of_outputs;
    }

    void setNoOfOutputs(const string &noOfOutputs) {
        no_of_outputs = noOfOutputs;
    }

    void setLocktime(const string &locktime) {
        UnsignedTxn::locktime = locktime;
    }

    void toString() {
        cout << "Electrum Partial Transaction Format: " << electrum_partial_tx;
        cout << "\nSeparator: " << separator;
        cout << "\nElectrum Partial Transaction Version: " << electrum_partial_tx_ver;
        cout << "\nNetwork Version: " << network_ver;
        cout << "\nNumber of Inputs: " << no_of_inputs;
        int nInputs = (int) strtol(no_of_inputs.c_str(), 0, 16);
        for (int i = 0; i < nInputs; i++) {
            cout << "\nPrevious Tx Hash: " << input.at(i).prev_tx_hash;
            cout << "\nPrevious Output Index: " << input.at(i).prev_output_index;
            cout << "\nScript Length: " << input.at(i).script_len;
            cout << "\nElectrum Specific: " << input.at(i).electron_specific;
            cout << "\nXpub Key Type: " << input.at(i).xpub_key_type;
            // Extended Public Key starts
            // see bip32.org
            cout << "\nSerialization Version Bytes: " << input.at(i).serial_ver_bytes;
            cout << "\nDepth: " << input.at(i).depth;
            cout << "\nFingerprint: " << input.at(i).fingerprint;
            cout << "\nChild Number: " << input.at(i).child_number;
            cout << "\nChain Code: " << input.at(i).chain_code;
            cout << "\nPublic Key: " << input.at(i).pub_key;
            cout << "\nDerivation: " << input.at(i).derivation;
            cout << "\nSequence: " << input.at(i).sequence;
            // Extended Public Key ends
        }
        cout << "\nNumber of Outputs: " << no_of_outputs;
        int nOutputs = (int) strtol(no_of_outputs.c_str(), 0, 16);
        for (int i = 0; i < nOutputs; i++) {
            cout << "\nAmount: " << output.at(i).amount;
            cout << "\nScript Length: " << output.at(i).script_len;
            cout << "\nScript Public Key: " << output.at(i).scriptPubKey.pub_key_hash;
        }
        cout << "\nLocktime: " << locktime;
    }
};

UnsignedTxn parser(string);

string getOutputHex(string no_of_outputs, vector<Output> output) {
    string outputHex;
    outputHex += no_of_outputs;
    int nOutputs = (int) strtol(no_of_outputs.c_str(), 0, 16);
    for (int i = 0; i < nOutputs; i++) {
        outputHex += output.at(i).amount;
        outputHex += output.at(i).script_len;
        outputHex += output.at(i).scriptPubKey.pub_key_hash;
    }
    return outputHex;
}

int main() {
    string hex;
    ifstream unsigned_txnFile("unsigned.txn");
    if (unsigned_txnFile.is_open()) {
        while (getline(unsigned_txnFile, hex)) {
            if (hex.substr(0, 9) == "    \"hex\"") {
                hex = hex.substr(12, 424);
                try {
                    if (hex.length() != 424) {
                        throw "Unsigned transaction not supported";
                    }
                } catch (const char *error) {
                    cout << error << endl;
                    exit(502);
                }
                break;
            }
        }
        unsigned_txnFile.close();
    } else {
        cout << "Unable to open unsigned transaction";
    }
    UnsignedTxn unsigned_txn = parser(hex);
    string prev_tx_hash = hex.substr(22, 64); // 64
    string prev_output_index = hex.substr(86, 8); // 8
    string inputScriptLen = "19";
    string inputScriptPubKey = "76a914c70279f1b15f90fb719689463621212da484763b88ac"; // https://api.blockcypher.com/v1/btc/test3/txs/2fe5b9e11e6fc335c711a14b81268cc4fa3c8807623f913760d341293a37f17d?limit=50&includeHex=true output:script
    const string inputPrivateKey = "cTm1JCUwQydQMW6yQ6wwatKtXGAQXjQv4cgZos9BDLYWSxLj9FUi"; // WIF
    string sigHashCode = "01000000"; // 8
    string raw_hex = unsigned_txn.getNetworkVer() + unsigned_txn.getNoOfInputs() + prev_tx_hash + prev_output_index +
                     inputScriptLen + inputScriptPubKey + hex.substr(270, 8) + // sequence
                     getOutputHex(unsigned_txn.getNoOfOutputs(), unsigned_txn.getOutput()) +
                     unsigned_txn.getLocktime() +
                     sigHashCode;
    uint8_t hash[32];
    size_t len = raw_hex.length();
    size_t tst = sha256((uint8_t *)raw_hex.c_str(), len, hash);
    cout << raw_hex << endl;
    for(int i = 0; i < 32; i++) {
        printf("%hhX", hash[i]);
    }
    return 0;
}

UnsignedTxn parser(string hex) {
    UnsignedTxn newUnsignedTxn;
    int lengths[] = {8, 2, 2, 8, 2, 64, 8, 2, 8, 2, 8, 2, 8, 8, 64, 66, 8, 8, 2, 16, 2, 50, 8};
    int startIndex = 0;
    newUnsignedTxn.setElectrumPartialTx(hex.substr(startIndex, lengths[0])); // 8
    startIndex += lengths[0];
    newUnsignedTxn.setSeparator(hex.substr(startIndex, lengths[1])); // 2
    startIndex += lengths[1];
    newUnsignedTxn.setElectrumPartialTxVer(hex.substr(startIndex, lengths[2])); // 2
    startIndex += lengths[2];
    newUnsignedTxn.setNetworkVer(hex.substr(startIndex, lengths[3])); // 8
    startIndex += lengths[3];
    newUnsignedTxn.setNoOfInputs(hex.substr(startIndex, lengths[4])); // 2
    int nInputs = (int) strtol(newUnsignedTxn.getNoOfInputs().c_str(), 0, 16);
    startIndex += lengths[4];
    vector<Input> inputs;
    for (int i = 1; i <= nInputs; i++) {
        Input newInput;
        newInput.prev_tx_hash = hex.substr(startIndex, lengths[5]); // 64
        startIndex += lengths[5];
        newInput.prev_output_index = hex.substr(startIndex, lengths[6]); // 8
        startIndex += lengths[6];
        newInput.script_len = hex.substr(startIndex, lengths[7]); // 2
        startIndex += lengths[7];
        newInput.electron_specific = hex.substr(startIndex, lengths[8]); // 8
        startIndex += lengths[8];
        newInput.xpub_key_type = hex.substr(startIndex, lengths[9]); // 2
        startIndex += lengths[9];
        newInput.serial_ver_bytes = hex.substr(startIndex, lengths[10]); // 8
        startIndex += lengths[10];
        newInput.depth = hex.substr(startIndex, lengths[11]); // 2
        startIndex += lengths[11];
        newInput.fingerprint = hex.substr(startIndex, lengths[12]); // 8
        startIndex += lengths[12];
        newInput.child_number = hex.substr(startIndex, lengths[13]); // 8
        startIndex += lengths[13];
        newInput.chain_code = hex.substr(startIndex, lengths[14]); // 64
        startIndex += lengths[14];
        newInput.pub_key = hex.substr(startIndex, lengths[15]); // 66
        startIndex += lengths[15];
        newInput.derivation = hex.substr(startIndex, lengths[16]); // 8
        startIndex += lengths[16];
        newInput.sequence = hex.substr(startIndex, lengths[17]); // 8
        startIndex += lengths[17];
        inputs.push_back(newInput);
    }
    newUnsignedTxn.setInput(inputs);
    newUnsignedTxn.setNoOfOutputs(hex.substr(startIndex, lengths[18])); // 2
    int nOutputs = (int) strtol(newUnsignedTxn.getNoOfOutputs().c_str(), 0, 16);
    startIndex += lengths[18];
    vector<Output> outputs;
    for (int i = 1; i <= nOutputs; i++) {
        Output newOutput;
        newOutput.amount = hex.substr(startIndex, lengths[19]); // 16
        startIndex += lengths[19];
        newOutput.script_len = hex.substr(startIndex, lengths[20]); // 2
        startIndex += lengths[20];
        ScriptPubKey newScriptPubKey;
        newScriptPubKey.pub_key_hash = hex.substr(startIndex, lengths[21]); // 40
        startIndex += lengths[21];
        newOutput.scriptPubKey = newScriptPubKey;
        outputs.push_back(newOutput);
    }
    newUnsignedTxn.setOutput(outputs);
    newUnsignedTxn.setLocktime(hex.substr(startIndex, lengths[22])); // 8
    startIndex += lengths[22];
    return newUnsignedTxn;
}
