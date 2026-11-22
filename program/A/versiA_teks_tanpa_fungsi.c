#include <stdio.h>

#define TEXT_SIZE 256
#define KEY_SIZE 128

static int my_strlen(const char *text)
{
    const char *cursor = text;

    if (text == NULL) {
        return 0;
    }

    while (*cursor != '\0') {
        cursor++;
    }

    return (int) (cursor - text);
}

static void my_memset(unsigned char *buffer, unsigned char value, int length)
{
    unsigned char *cursor = buffer;
    unsigned char *end = buffer + length;

    while (cursor < end) {
        *cursor = value;
        cursor++;
    }
}

static void trim_newline(char *text)
{
    char *cursor = text;

    while (*cursor != '\0') {
        if (*cursor == '\n' || *cursor == '\r') {
            *cursor = '\0';
            return;
        }
        cursor++;
    }
}

static void xor_text(
    const unsigned char *input,
    unsigned char *output,
    int text_length,
    const unsigned char *key,
    int key_length,
    int *key_position
)
{
    const unsigned char *input_cursor = input;
    const unsigned char *input_end = input + text_length;
    unsigned char *output_cursor = output;
    const unsigned char *key_cursor = key + *key_position;
    const unsigned char *key_end = key + key_length;

    while (input_cursor < input_end) {
        *output_cursor = (unsigned char) (*input_cursor ^ *key_cursor);
        input_cursor++;
        output_cursor++;
        key_cursor++;

        if (key_cursor == key_end) {
            key_cursor = key;
        }
    }

    *key_position = (int) (key_cursor - key);
}

static void print_hex(const unsigned char *data, int length)
{
    const unsigned char *cursor = data;
    const unsigned char *end = data + length;

    while (cursor < end) {
        printf("%02X ", *cursor);
        cursor++;
    }
    printf("\n");
}

int main(void)
{
    unsigned char plaintext[TEXT_SIZE];
    unsigned char ciphertext[TEXT_SIZE];
    unsigned char decrypted[TEXT_SIZE];
    unsigned char key[KEY_SIZE];
    int text_length;
    int key_length;
    int key_position = 0;

    my_memset(plaintext, 0, TEXT_SIZE);
    my_memset(ciphertext, 0, TEXT_SIZE);
    my_memset(decrypted, 0, TEXT_SIZE);
    my_memset(key, 0, KEY_SIZE);

    printf("Masukkan plaintext: ");
    if (fgets((char *) plaintext, TEXT_SIZE, stdin) == NULL) {
        return 1;
    }
    trim_newline((char *) plaintext);

    printf("Masukkan key: ");
    if (fgets((char *) key, KEY_SIZE, stdin) == NULL) {
        return 1;
    }
    trim_newline((char *) key);

    text_length = my_strlen((const char *) plaintext);
    key_length = my_strlen((const char *) key);

    if (text_length == 0 || key_length == 0) {
        printf("Plaintext dan key tidak boleh kosong.\n");
        return 1;
    }

    xor_text(
        plaintext,
        ciphertext,
        text_length,
        key,
        key_length,
        &key_position
    );

    printf("Ciphertext (hex): ");
    print_hex(ciphertext, text_length);

    key_position = 0;
    xor_text(
        ciphertext,
        decrypted,
        text_length,
        key,
        key_length,
        &key_position
    );
    decrypted[text_length] = '\0';

    printf("Hasil dekripsi: %s\n", decrypted);

    my_memset(key, 0, KEY_SIZE);
    return 0;
}
