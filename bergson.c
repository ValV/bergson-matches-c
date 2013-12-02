#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define	BM_MAX_MATCHES		50
#define	BM_FIRST_STEP		2UL
#define	bm_min( arg1, arg2 )	( arg1 < arg2 ) ? arg1 : arg2
#define	bm_max( arg1, arg2 )	( arg1 > arg2 ) ? arg1 : arg2
#define	bm_binet( arg1 )		( int ) round( pow( 1 + pow( 5, 0.5), arg1 ) / pow( 5, 0.5) )
#define	bm_debug			1

/* I18n strings */
#define	STR_ERR_PLAYER		"\nError: wrong player in turn!\n"
#define	STR_ERR_OTHER		"\nError: unhandled error...\n"
#define	STR_ERR_INPUT		"\nError: wrong number.\nPlease, enter positive number ('0', 'exit' to end)\n"
#define	STR_TODO			"Not realized yet. To be done.\n"
#define	STR_PLAYER0_TURN	"\nThis is your turn. Pick matches. Maximum: "
#define	STR_PLAYER1_TURN	"\n-= Player 1 =- can take. Maximum: "
#define	STR_PLAYER2_TURN	"\n.::Player 2::. can take. Maximum: "
#define	STR_AI_TURN			"\nNow, it is my turn. I will take. Maximum: "
#define	STR_WINNER			"\nWinner! Player %i\n"
#define	STR_WIN_PLAYER		"\nYOU have WON! But this time.\nGo another try!\n"
#define	STR_WIN_AI			"\nAha! I have WON!\nYou can not defeat Artificial Intelligence\n"
#define	STR_MATCHES			"\n\nMatches remain: %lu\n"
#define	STR_UNLIMITED		"unlimited\n"

volatile unsigned long bm_limit = 0, bm_remain = 0;
volatile unsigned char bm_player = 0;
volatile unsigned long bm_fibsz = 0;
unsigned long *bm_fibonacci = NULL, *bm_fibtmp;

void echo_matches( void );
void echo_player_pickup( unsigned char num );
void ai_pickup( void );
unsigned long ai_decide( unsigned long ai_remain, unsigned long ai_limit );
unsigned char player_pickup( unsigned char num );
void pvp_loop( unsigned char forever );
void pve_loop( unsigned char forever );
unsigned long player_input( void );

int main( void ) {
	unsigned char keypress;
	/* Main menu */
	do {
		printf( "\n\n    .::Main menu::.\n" );
		printf( "1 - player vs ai\n" );
		printf( "2 - player vs player\n" );
		printf( "3 - player vs ai (permanent)\n" );
		printf( "4 - player vs player (permanent)\n" );
		printf( "0 - exit (type '0' or 'exit')\n" );
		printf( "> " );
		//scanf( "%c",&keypress );
		keypress = ( unsigned char ) player_input();
		bm_limit = BM_FIRST_STEP;
		

		do {
			//
			bm_fibsz = 0;
			bm_fibtmp = ( unsigned long * ) realloc( bm_fibonacci,
						( bm_fibsz + 1 ) * sizeof(unsigned long));
			if ( bm_fibtmp == NULL) {
				printf( "Error allocating memory\n" );
				keypress = 0;
				break;
			}
			bm_fibonacci = bm_fibtmp;
			bm_fibonacci[bm_fibsz] = bm_binet(bm_fibsz);
			if ( bm_debug ) printf( "%lu\n", bm_fibonacci[bm_fibsz] );
			bm_fibsz ++;
		} while (bm_fibonacci[bm_fibsz] <= BM_MAX_MATCHES );

		switch ( keypress ) {
			case 1: pve_loop( 0 ); break;
			case 2: pvp_loop( 0 ); break;
			case 3: pve_loop( 1 ); break;
			case 4: pvp_loop( 1 ); break;
			default: break;
		}
		//printf( "Number of pressed key = %u\n", keypress );
	} while ( keypress > 0 );
	return ( 0 );
}

void echo_matches( void ) {
	unsigned long i = 0;
	printf( STR_MATCHES, bm_remain );
	/* Error handling condition for unsigned (if any) */
	if ( bm_remain > BM_MAX_MATCHES ) {
		printf( STR_ERR_OTHER );
		return;
	}
	for ( ; i < bm_remain; i ++ ) printf( "|" );
	printf( "\n" );
}

void echo_player_pickup( unsigned char num ) {
	switch ( num ) {
		case 0: printf( STR_PLAYER0_TURN ); break;
		case 1: printf( STR_PLAYER1_TURN ); break;
		case 2: printf( STR_PLAYER2_TURN ); break;
		default: printf( STR_ERR_PLAYER ); break;
	}
	if ( bm_limit ) printf( "%lu\n" , bm_limit );
	else printf( STR_UNLIMITED );
}

void ai_pickup( void ) {
	/* This function is not realized yet */
	unsigned char bm_rand = 0;
	unsigned long bm_ai_take = 0;

	printf( STR_AI_TURN );
	if ( bm_limit ) printf( "%lu\n" , bm_limit );
	else printf( STR_UNLIMITED );

	srandom( time( NULL ) );
	bm_rand = random() % 4 + 1;
	//printf( "\nRandom roll: %u\n", bm_rand );

	/* Artificial Choice */
	if ( bm_remain > 12 ) bm_ai_take = bm_min( bm_limit, bm_rand );
	else {
		/*  */
		if ( bm_remain > 8 )
			bm_ai_take = bm_min( bm_limit, bm_remain - 8 );
		else
			if ( bm_remain <= bm_limit ) {
				//bm_matches_remain = ai_pickup;
				//break;
			}
			else ;
	}
	bm_remain -= ( ! bm_limit ) ? bm_remain : bm_ai_take; 
	bm_limit = ( ( bm_ai_take * 2 ) <= bm_remain ) ?
			( bm_ai_take * 2 ) : bm_remain;
}

/* Recursive function of AI choice */
unsigned long ai_decide( unsigned long ai_remain, unsigned long ai_limit ) {

	return ( 0 );
}

unsigned long player_input( void ) {
	unsigned long bm_input = 0;
	int /*bm_debug = 0,*/ bm_i = 0;
	char *bm_inbuf = ( char * ) malloc( 4 ), *bm_exit = "exit";
	/* Error handling cycle */
	while ( ( bm_i = scanf( "%lu", &bm_input ) ) < 1 ) {// EOL = -1, if read error occure...
		// scanf( "%*[^\n]" );
		scanf( "%4s", bm_inbuf );
		scanf( "%*[^\n]" ); // Clear scanf input buffer
		/* Exit condition */
		if ( bm_inbuf[0] == bm_exit[0]
			&& bm_inbuf[1] == bm_exit[1]
			&& bm_inbuf[2] == bm_exit[2]
			&& bm_inbuf[3] == bm_exit[3] ) {
			//scanf( "%*[^\n]" ); // Clear scanf input buffer
			return ( 0 );
		}
		printf( STR_ERR_INPUT );
	} // Get rid of wrong buffer with warning
	//printf( "Scanf result = %d\n", bm_debug ); // Check values've been got
	return ( bm_input );
}

unsigned char player_pickup( unsigned char num ) {
	unsigned long bm_player_take;
	do {
		echo_player_pickup( num );
		bm_player_take = player_input();
		/* If player entered '0' or 'exit' - end */
		if ( bm_player_take == 0 ) {
			bm_remain = 0;
			bm_limit = 1;
			return ( bm_player_take );
		}
	} while ( ( bm_player_take > bm_limit ) && bm_limit );
	bm_remain -= ( ! bm_limit ) ? bm_remain : bm_player_take; 
	bm_limit = ( ( bm_player_take * 2 ) <= bm_remain ) ?
			( bm_player_take * 2 ) : bm_remain;
	/**/
	switch ( num ) {
		case 1: break;
		case 2: break;
		case 0: break;
		default: printf( STR_ERR_OTHER ); break;
	}
	//printf( "Player %u: matches remain %u, last turn %u\n", num, bm_matches_remain, bm_last_turn );
	return ( 1 );
}

void pvp_loop( unsigned char forever ) {
	do {
		bm_player = 1;
		bm_remain = BM_MAX_MATCHES;
		while ( bm_remain > 0 ) {
			echo_matches();
			//echo_player_take( bm_player );
			if ( ! player_pickup( bm_player ) ) forever = 0;
			if ( bm_limit ) bm_player = ( bm_player & 1 ) ? 2 : 1;
		}
		printf( STR_WINNER, bm_player );
		//printf( "%i\n", bm_player );
		//scanf( "%*[^\n]" ); // Clear scanf input buffer
	} while ( forever );
}

void pve_loop( unsigned char forever ) {
	do {
		bm_player = 0;
		bm_remain = BM_MAX_MATCHES;
		while ( bm_remain > 0 ) {
			echo_matches();
			//echo_player_take( bm_player );
			printf( "\nPlayer: %u\n", bm_player );
			if ( bm_player ) {
				if ( ! player_pickup( 0 ) ) forever = 0;
			}
			else ai_pickup();
			if ( ! bm_remain ) break;
			bm_player ++;
			bm_player = bm_player & 0x01;
		}
		if ( bm_player ) printf( STR_WIN_PLAYER );
		else printf( STR_WIN_AI );
		//printf( "%i\n", bm_player );
		//scanf( "%*[^\n]" ); // Clear scanf input buffer
	} while ( forever );
}
