const assert = require('assert')
const {dialog} = require('electron').remote

describe('dialog module', () => {
  describe('showOpenDialog', () => {
    it('throws errors when the options are invalid', () => {
      assert.throws(() => {
        dialog.showOpenDialog({properties: false})
      }, /Properties must be an array/)

      assert.throws(() => {
        dialog.showOpenDialog({title: 300})
      }, /Title must be a string/)

      assert.throws(() => {
        dialog.showOpenDialog({buttonLabel: []})
      }, /Button label must be a string/)

      assert.throws(() => {
        dialog.showOpenDialog({defaultPath: {}})
      }, /Default path must be a string/)
    })
  })

  describe('showSaveDialog', () => {
    it('throws errors when the options are invalid', () => {
      assert.throws(() => {
        dialog.showSaveDialog({title: 300})
      }, /Title must be a string/)

      assert.throws(() => {
        dialog.showSaveDialog({buttonLabel: []})
      }, /Button label must be a string/)

      assert.throws(() => {
        dialog.showSaveDialog({defaultPath: {}})
      }, /Default path must be a string/)
    })
  })

  describe('showMessageBox', () => {
    it('throws errors when the options are invalid', () => {
      assert.throws(() => {
        dialog.showMessageBox(undefined, {type: 'not-a-valid-type'})
      }, /Invalid message box type/)

      assert.throws(() => {
        dialog.showMessageBox(null, {buttons: false})
      }, /Buttons must be an array/)

      assert.throws(() => {
        dialog.showMessageBox({title: 300})
      }, /Title must be a string/)

      assert.throws(() => {
        dialog.showMessageBox({message: []})
      }, /Message must be a string/)

      assert.throws(() => {
        dialog.showMessageBox({detail: 3.14})
      }, /Detail must be a string/)
    })
  })

  describe('showErrorBox', () => {
    it('throws errors when the options are invalid', () => {
      assert.throws(() => {
        dialog.showErrorBox()
      }, /Insufficient number of arguments/)

      assert.throws(() => {
        dialog.showErrorBox(3, 'four')
      }, /Error processing argument at index 0/)

      assert.throws(() => {
        dialog.showErrorBox('three', 4)
      }, /Error processing argument at index 1/)
    })
  })
})
