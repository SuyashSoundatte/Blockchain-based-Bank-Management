const { GenesisBlock, MINE_RATE } = require("./config");
const cryptoHash = require("./crypto-hash");
const hexToBinary = require('hex-to-binary');

class Block {
    constructor({ timeStamp, data, hash, prevHash, nonce, difficulty }) {
        this.timeStamp = timeStamp;
        this.data = data;
        this.prevHash = prevHash;
        this.hash = hash;
        this.nonce = nonce;
        this.difficulty = difficulty;
    }

    // Method to create the genesis block
    static genesis() {
        return new this(GenesisBlock);
    }

    // Method to mine a new block
    static mineBlock({ data, prevBlock }) {
        let hash, timeStamp;
        const prevHash = prevBlock.hash;
        let { difficulty } = prevBlock;

        let nonce = 0;
        do {
            nonce++;
            timeStamp = Date.now(); // Use Date.now() to get numeric timestamp
            difficulty = Block.adjustDifficulty({ 
                originalBlock: prevBlock, 
                timeStamp
            });
            hash = cryptoHash(timeStamp, prevHash, data, nonce, difficulty);
        } while (hexToBinary(hash).substring(0, difficulty) !== '0'.repeat(difficulty));

        return new Block({
            timeStamp,
            prevHash,
            data,
            hash,
            nonce,
            difficulty
        });
    }

    static adjustDifficulty({ originalBlock, timeStamp }) {
        let { difficulty } = originalBlock;
        if (difficulty < 1) {
            return 1;
        }
        const difference = timeStamp - originalBlock.timeStamp;

        if (difference > MINE_RATE) {
            return difficulty - 1;
        }
        return difficulty + 1;
    }

    static isValidChain(chain) {
        if (JSON.stringify(chain[0]) !== JSON.stringify(Block.genesis())) {
            return false;
        }

        for (let i = 1; i < chain.length; i++) {
            const { timeStamp, data, prevHash, hash, nonce, difficulty } = chain[i];
            const realLastHash = chain[i - 1].hash;
            const lastDifficulty = chain[i - 1].difficulty;

            if (prevHash !== realLastHash) {
                return false;
            }

            const validatedHash = cryptoHash(timeStamp, prevHash, data, nonce, difficulty);
            if (hash !== validatedHash) {
                return false;
            }

            if (Math.abs(lastDifficulty - difficulty) > 1) {
                return false;
            }
        }

        return true;
    }
}

module.exports = Block;
