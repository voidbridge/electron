const crypto = require('crypto')
const fs = require('fs')
const path = require('path')

// Fallback to blow away old cache keys
const HASH_VERSION = 1

// Base files to hash
const filesToHash = [
  path.resolve(__dirname, '../DEPS'),
  path.resolve(__dirname, '../yarn.lock')
]

const addAllFiles = (dir) => {
  for (const child of fs.readdirSync(dir).sort()) {
    const childPath = path.resolve(dir, child)
    if (fs.statSync(childPath).isDirectory()) {
      addAllFiles(childPath)
    } else {
      filesToHash.push(childPath)
    }
  }
}

// Add all patch files to the hash
addAllFiles(path.resolve(__dirname, '../patches'))

// Create Hash
const hasher = crypto.createHash('SHA256')
for (const file of filesToHash) {
  hasher.update(fs.readFileSync(file))
}

// Add the GCLIENT_EXTRA_ARGS variable to the hash
hasher.update(process.env.GCLIENT_EXTRA_ARGS || 'no_extra_args')

// Write the hash to disk
fs.writeFileSync(path.resolve(__dirname, '../.depshash'), hasher.digest('hex'))
