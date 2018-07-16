
#ifndef GOKIBURI_H
#define GOKIBURI_H

#include <bstrlib.h>

enum LG_COLOR {
   LG_COLOR_DARK_BLUE   =  1,
   LG_COLOR_DARK_GREEN  =  2,
   LG_COLOR_DARK_CYAN   =  3,
   LG_COLOR_DARK_RED    =  4,
   LG_COLOR_PURPLE      =  5,
   LG_COLOR_BROWN       =  6,
   LG_COLOR_GRAY        =  7,
   LG_COLOR_CHARCOAL    =  8,
   LG_COLOR_BLUE        =  9,
   LG_COLOR_GREEN       = 10,
   LG_COLOR_CYAN        = 11,
   LG_COLOR_RED         = 12,
   LG_COLOR_MAGENTA     = 13,
   LG_COLOR_YELLOW      = 14,
   LG_COLOR_WHITE       = 15
};

enum LGC_ERROR {
   LGC_ERROR_NONE,
   LGC_ERROR_NULLPO
};

#define lgc_silence() lgc_error_silent = TRUE;
#define lgc_unsilence() lgc_error_silent = FALSE;

#define lgc_null_msg( pointer, message ) \
    if( NULL == pointer ) { \
        scaffold_error = LGC_ERROR_NULLPO; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, \
               "Scaffold: Null pointer on line: %d: %s\n", \
               __LINE__, message ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_null_warning( pointer ) \
    if( NULL == pointer ) { \
        scaffold_error = LGC_ERROR_NULLPO; \
        if( TRUE != scaffold_warning_silent ) { \
            scaffold_print_warning( \
               &module, "Scaffold: Null pointer on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_null( pointer ) \
    if( NULL == pointer ) { \
        scaffold_error = LGC_ERROR_NULLPO; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, "Scaffold: Null pointer on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_null_continue( pointer ) \
    if( NULL == pointer ) { \
        scaffold_error = LGC_ERROR_NULLPO; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, "Scaffold: Null pointer on line: %d\n", __LINE__ ); \
            scaffold_print_debug( &module, "Continuing loop..." ); \
        } \
        continue; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_not_null( pointer ) \
    if( NULL != pointer ) { \
        scaffold_error = LGC_ERROR_NOT_NULLPO; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, \
               "Scaffold: Non-null pointer on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_bounds( index, bound ) \
    if( index >= bound ) { \
        scaffold_error = LGC_ERROR_OUTOFBOUNDS; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, "Scaffold: Out of bounds on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_negative( value ) \
    if( 0 > value ) { \
        scaffold_error = LGC_ERROR_NEGATIVE; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, "Scaffold: Bad negative on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_nonzero( value ) \
    if( 0 != value ) { \
        scaffold_error = LGC_ERROR_NONZERO; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, "Scaffold: Nonzero error on line: %d\n", __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_zero_msg( value, message ) \
    if( 0 == value ) { \
        scaffold_error = LGC_ERROR_ZERO; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, \
               "Scaffold: Zero error on line: %d: %s\n", __LINE__, message ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }


#define lgc_zero_against_warning( last, value, msg ) \
    if( 0 == value && LGC_ERROR_ZERO != last ) { \
        last = LGC_ERROR_ZERO; \
        if( TRUE != scaffold_warning_silent ) { \
            scaffold_print_warning( \
               &module, \
               "Scaffold: Zero warning on line: %d: %s\n", __LINE__, msg ); \
        } \
        goto cleanup; \
    } else if( LGC_ERROR_ZERO != last ) { \
        last = LGC_ERROR_NONE; \
    }

#define lgc_zero_against( last, value, msg ) \
    if( 0 == value && LGC_ERROR_ZERO != last ) { \
        last = LGC_ERROR_ZERO; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module, \
               "Scaffold: Zero error on line: %d: %s\n", __LINE__, msg ); \
        } \
        goto cleanup; \
    } else if( LGC_ERROR_ZERO != last ) { \
        last = LGC_ERROR_NONE; \
    }

#define lgc_equal( value1, value2 ) \
    if( value1 != value2 ) { \
        scaffold_error = LGC_ERROR_UNEQUAL; \
        if( TRUE != lgc_error_silent ) { \
            scaffold_print_error( \
               &module,\
               "Values not equal: %d and %d: error on line: %d\n", \
               value1, value2, __LINE__ ); \
        } \
        goto cleanup; \
    } else { \
        scaffold_error = LGC_ERROR_NONE; \
    }

#define lgc_zero( value, msg ) \
   lgc_zero_against( scaffold_error, value, msg )


#ifdef USE_LOG_FILE
#ifndef GOKIBURI_C
extern FILE* lg_handle_out;
extern FILE* lg_handle_err;
#endif /* GOKIBURI_C */
#else
#define lg_handle_out stdout
#define lg_handle_err stderr
#endif /* USE_LOG_FILE */

#endif /* GOKIBURI_H */
