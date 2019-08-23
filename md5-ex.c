const char *md5sum(const char *chaine, size_t len = 0)
 {
     struct md5_ctx ctx;
     unsigned char digest(16);
     md5_init(&ctx);
     ctx.size = len?len:strlen(chaine);
     strcpy(ctx.buf, chaine);
     // or:  strndup(ctx.buf, chaine, strlen(chaine))
     md5_update(&ctx);
     md5_final(digest, &ctx);
     return digest;
 }
