//Rhino 1.7.7.1

var chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

function index(c)
{
    for (ii = 0; ii < chars.length; ++ii)
    {
        if(chars[ii] == c) {

            return ii;
        }
    }

    return -1;
}

var alphas = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
var digits = "0123456789";

function isAlpha(c) {
return (alphas.indexOf(c) != -1);
}

function isDigit(c) {
return (digits.indexOf(c) != -1);
}

function isAlphaNum(c) {

return (isAlpha(c) || isDigit(c));
}

String.prototype.replaceAt=function(index, replacement) {
  return this.substr(0, index) + replacement+ this.substr(index + replacement.length);
}

function extend_key(msg, key)
{
    var msgLen = msg
    var niga = "x";

    var newKey = niga.repeat(msgLen);

    var keyLen = key.legnth
    
    var newKeyss = "";
    
    for (i = 0, j = 0; i < msgLen; ++i, ++j){
        if (j == Number(keyLen)){ 
            j = 0; 
            print("YES")
        }
        newKey = newKey.replaceAt(i, key[j]);

    }
    newKey[i] = '\0';
    return newKey;
}

function encrypt_vigenere(msg, key)
{
    var msgLen = Number(msg.length);
    var keyLen = Number(key.length), i, j;
    var encryptedMsg = 'x'.repeat(msgLen);
    
    var newKey = extend_key(msgLen, key);

    
    for( i = 0; i < msgLen; ++i){
        if ( isAlphaNum(msg[i]) || msg[i] == ' '){
            encryptedMsg = encryptedMsg.replaceAt(i, chars[((index(msg[i]) + index(newKey[i])) % chars.length)])
        }else{
            
              encryptedMsg = encryptedMsg.replaceAt(i,msg[i]);
        }
    }
    encryptedMsg[i] = '\0';
    return encryptedMsg;
    
}

function decrypt_vigenere(encryptedMsg, newKey)
{
    var msgLen = encryptedMsg.length;
    
    var decryptedMsg = 'x'.repeat(msgLen)
    var i;
    
    for(i = 0; i < msgLen; ++i) {
        if(isAlphaNum(encryptedMsg[i]) || encryptedMsg[i] == ' '){
            decryptedMsg = decryptedMsg.replaceAt(i, chars[(((index(encryptedMsg[i]) - index(newKey[i])) + chars.length) % chars.length)])
        }else{
            decryptedMsg = decryptedMsg.replaceAt(i, encryptedMsg[i]);
        }
    }
    decryptedMsg[i] = '\0';
    return decryptedMsg;
}

var keycrypt = "DLq7lMCcuMIIVD13MRAuLv7H2fdW9f6g";
var keys = "QQ5CjZu3kEbHDjeBBFnR0";

var hwid = "IG0vdTkXbw";

var fucker = decrypt_vigenere(hwid, keys);

print(fucker)

var crypt = keys + fucker;



var zalupa = encrypt_vigenere(crypt, keycrypt);

print(zalupa)