extern long debug_flags;

#define DEBUG_HASH 0x01
#define DEBUG_PARSE 0x02
#define DEBUG_DICT 0x04
#define DEBUG_COMPRESS 0x08

#define DEBUG_DICT_DECODE 0x01
#define DEBUG_DECOMPRESS 0x02
#define DEBUG_DECODE 0x04


#define DICT_DEBUG_ENABLED (debug_flags & DEBUG_DICT)
#define HASH_DEBUG_ENABLED (debug_flags & DEBUG_HASH)
#define PARSE_DEBUG_ENABLED (debug_flags & DEBUG_PARSE)
#define COMPRESS_DEBUG_ENABLED (debug_flags & DEBUG_COMPRESS)

#define DICT_DECODE_DEBUG_ENABLED (debug_flags & DEBUG_DICT_DECODE)
#define DECOMPRESS_DEBUG_ENABLED (debug_flags & DEBUG_DECOMPRESS)
#define DECODE_DEBUG_ENABLED (debug_flags & DEBUG_DECODE)
