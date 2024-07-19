const GenesisBlock = {
    timeStamp: Date.now(),
    data: "Genesis block",
    prevHash: "0x000",
    hash: "0x123",
    nonce: 0,
    difficulty: 3
};

const MINE_RATE = 1000; // 1 second

module.exports = { GenesisBlock, MINE_RATE };
