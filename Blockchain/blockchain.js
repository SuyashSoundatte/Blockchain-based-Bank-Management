const Block = require('./block');
const cryptoHash = require('./crypto-hash');

class Blockchain {
    constructor() {
        this.chain = [Block.genesis()];
    }

    addBlock({ data }) {
        const newBlock = Block.mineBlock({
            prevBlock: this.chain[this.chain.length - 1],
            data: data
        });
        this.chain.push(newBlock);
    }

    replaceChain(chain) {
        if (chain.length <= this.chain.length) {
            console.error("The incoming chain is not longer");
            return;
        }
        if (!Blockchain.isValidChain(chain)) {
            console.error("The incoming chain is not valid");
            return;
        }
        console.log("Replacing chain with", chain);
        this.chain = chain;
    }

    static isValidChain(chain) {
        return Block.isValidChain(chain);
    }
}

// Remove the following block creation code if used for testing only
const block1 = new Block({
    hash: "0xabc",
    timeStamp: Date.now(),
    prevHash: "0xxyz",
    data: "Block 1",
});

const block2 = new Block({
    hash: "0xabc",
    timeStamp: Date.now(),
    prevHash: "0xxyz",
    data: "Block 2",
});

const btc = new Blockchain();
btc.addBlock({ data: block1 });
console.log(btc);
btc.addBlock({ data: block2 });
console.log(btc);

module.exports = Blockchain;
