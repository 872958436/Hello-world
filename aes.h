#ifndef _AES_H
#define _AES_H

extern uint8_t *w;
void init_environment();
void key_expansion(uint8_t *key, uint8_t *w);
void cipher(uint8_t *in, uint8_t *out, uint8_t *w);
void inv_cipher(uint8_t *in, uint8_t *out, uint8_t *w);

#endif
