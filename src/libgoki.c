
#define GOKIBURI_C
#include "libgoki.h"

#ifdef DEBUG
#include <assert.h>
#endif /* DEBUG */

#define LG_BUFFER_ALLOC 255

static bstring lg_buffer = NULL;

static void lg_log(
   void* log, const bstring mod_in, enum LG_COLOR color,
   const char* message, va_list varg
) {
   int bstr_ret;

   if( NULL == lg_buffer ) {
      lg_buffer = bfromcstralloc( LG_BUFFER_ALLOC, "" );
   }
#ifdef DEBUG
   assert( NULL != lg_buffer );
#endif /* DEBUG */

   bstr_ret = btrunc( lg_buffer, 0 );
   lgc_nonzero( bstr_ret );

#ifdef DEBUG
   scaffold_snprintf(
      lg_buffer, "%b (%b): ",
      mod_in, &(str_scaffold_trace[scaffold_trace_path])
   );
   scaffold_vsnprintf( lg_buffer, message, varg );
#else
   scaffold_snprintf( lg_buffer, "%b: ", mod_in );
   scaffold_vsnprintf( lg_buffer, message, varg );
#endif /* DEBUG */

   scaffold_colorize( lg_buffer, color );

   fprintf( log, "%s", bdata( lg_buffer ) );

cleanup:
   return;
}

void lg_debug( const bstring mod_in, const char* message, ... ) {
#ifdef DEBUG
   va_list varg;
   LG_COLOR color;
#ifndef HEATSHRINK_DEBUGGING_LOGS
   const char* mod_in_c = NULL;

   mod_in_c = bdata( mod_in );

   /* How's this for an innovative solution to keeping it C89? */
   if( 0 == strncmp( "hs", mod_in_c, 2 ) ) {
      goto cleanup;
   } if( 0 == strncmp( "syncbuff", mod_in_c, 8 ) ) {
      goto cleanup;
   }
#endif /* HEATSHRINK_DEBUGGING_LOGS */

   if( SCAFFOLD_TRACE_SERVER == scaffold_trace_path ) {
      color = LG_COLOR_MAGENTA;
   } else {
      color = LG_COLOR_CYAN;
   }

   va_start( varg, message );
   scaffold_log(
      scaffold_log_handle_err, mod_in, color, message, varg
   );
   va_end( varg );

cleanup:
   return;
#endif /* DEBUG */
}

void lg_color(
   const bstring mod_in, LG_COLOR color, const char* message, ...
) {
#ifdef DEBUG
   va_list varg;
   va_start( varg, message );
   scaffold_log( scaffold_log_handle_err, mod_in, color, message, varg );
   va_end( varg );
   return;
#endif /* DEBUG */
}

void lg_info( const bstring mod_in, const char* message, ... ) {
   va_list varg;
   int bstr_ret;

   if( NULL == lg_buffer ) {
      lg_buffer = bfromcstralloc( SCAFFOLD_PRINT_BUFFER_ALLOC, "" );
   }
   scaffold_assert( NULL != lg_buffer );

   bstr_ret = btrunc( lg_buffer, 0 );
   lgc_nonzero( bstr_ret );

   va_start( varg, message );
   scaffold_vsnprintf( lg_buffer, message, varg );
   va_end( varg );

#ifdef DEBUG
   fprintf( scaffold_log_handle, "%s: %s",
      bdata( mod_in ), bdata( lg_buffer ) );
#endif /* DEBUG */

#ifndef DISABLE_BACKLOG
   backlog_system( lg_buffer );
#endif /* DISABLE_BACKLOG */

cleanup:
   return;
}

void lg_error( const bstring mod_in, const char* message, ... ) {
   va_list varg;
#ifdef DEBUG
   va_start( varg, message );
   scaffold_log(
      scaffold_log_handle_err, mod_in, LG_COLOR_RED, message, varg
   );
   va_end( varg );
#endif /* DEBUG */
}

void lg_warning( const bstring mod_in, const char* message, ... ) {
   va_list varg;
#ifdef DEBUG
   va_start( varg, message );
   scaffold_log(
      scaffold_log_handle_err, mod_in, LG_COLOR_YELLOW, message, varg
   );
   va_end( varg );
#endif /* DEBUG */
}

/*
void scaffold_snprintf( bstring buffer, const char* message, ... ) {
   va_list varg;
   va_start( varg, message );
   scaffold_vsnprintf( buffer, message, varg );
   va_end( varg );
}
*/

void lg_vsnprintf( bstring buffer, const char* message, va_list varg ) {
   const char* chariter;
   bstring insert = NULL;
   int bstr_res;
   int i;
   void* p;

   for( chariter = message ; '\0' != *chariter ; chariter++ ) {
      if( '%' != *chariter ) {
         bstr_res = bconchar( buffer, *chariter );
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         continue;
      }

      switch( *++chariter ) {
      case 'c':
         i = va_arg( varg, int );
         bstr_res = bconchar( buffer, i );
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case 'd':
         i = va_arg( varg, int );
         insert = bformat( "%d", i );
#ifdef DEBUG
         assert( NULL != insert );
#endif /* DEBUG */
         bstr_res = bconcat( buffer, insert );
         bdestroy( insert );
         insert = NULL;
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case 's':
         insert = bfromcstr( va_arg( varg, char*) );
#ifdef DEBUG
         assert( NULL != insert );
#endif /* DEBUG */
         bstr_res = bconcat( buffer, insert );
         bdestroy( insert );
         insert = NULL;
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case 'b':
         insert = va_arg( varg, bstring );
         /* TODO: (NULL) if null. */
         bstr_res = bconcat( buffer, insert );
         insert = NULL;
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case 'p':
         p = va_arg( varg, void* );
         insert = bformat( "%p", p );
#ifdef DEBUG
         assert( NULL != insert );
#endif /* DEBUG */
         bstr_res = bconcat( buffer, insert );
         bdestroy( insert );
         insert = NULL;
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;

      case '%':
         bstr_res = bconchar( buffer, '%' );
#ifdef DEBUG
         assert( 0 == bstr_res );
#endif /* DEBUG */
         break;
      }
   }

cleanup:
   bdestroy( insert );
   return;
}

#ifdef USE_COLORED_CONSOLE

void scaffold_colorize( bstring str, LG_COLOR color ) {
   bstring str_color = NULL;
   int color_i = (int)color;
   int bstr_ret;

   if( color_i > 6 ) {
      color_i -= 9;
   }
   if( 0 > color_i || 7 <= color_i ) {
      goto cleanup;
   }
   str_color = &(ansi_color_strs[color_i]);

   bstr_ret = binsert( str, 0, str_color, '\0' );
   if( 0 != bstr_ret ) {
      goto cleanup;
   }
   bstr_ret = bconcat( str, &(ansi_color_strs[LG_COLOR_WHITE - 9]) );
   if( 0 != bstr_ret ) {
      goto cleanup;
   }

cleanup:
   return;
}
