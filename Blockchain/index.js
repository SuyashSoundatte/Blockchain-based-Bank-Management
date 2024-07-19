require('dotenv').config();
const express = require('express');
const Blockchain = require('./blockchain');
const bodyParser = require('body-parser');

const app = express();
const btc = new Blockchain();

app.use(bodyParser.json());

app.get('/api/blocks', (req, res) => {
    res.json(btc.chain);
});

app.post("/api/mine", (req, res) => {
    const { data } = req.body;
    if (!data) {
        return res.status(400).json({ error: "Data is required to mine a block" });
    }
    btc.addBlock({ data }); // Ensure consistent naming
    res.redirect('/api/blocks');
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Example app listening on port ${PORT}`);
});
