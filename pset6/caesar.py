def cipher(plaintext, key):
    ciphertext = ""
    for char in plaintext:
        if char.isalpha():
            num = ord(char) + key
            if num > ord('z'):
                num -= 26
            ciphertext = ciphertext + chr(num)

    print(ciphertext)

print(cipher('Aello', 5))

