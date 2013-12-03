/* Bergson matches */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define bm_min( arg1, arg2 )	( arg1 < arg2 ) ? arg1 : arg2
#define bm_rndcomput( arg1 )	round ( pow( ( 1 + pow( 5, 0.5 ) ) / 2, arg1 ) / pow( 5, 0.5) )

/* I18n strings */
#define STR_ERR_WRPL	"\nОшибка: ошибка расчёта следующего хода!\n"
#define STR_ERR_UNKN	"\nОшибка: неизвестная ошибка...\n"
#define STR_ERR_INUM	"\nОшибка: неверное число.\nНужно ввести целое число ('0', 'exit' для завершения)\n"
#define STR_ERR_MALL	"\nОшибка: ошибка выделения памяти\n"

#define STR_ACT_PL0T	"\nТвой ход. Тяни спички. Можно взять: "
#define STR_ACT_PL1T	"\n-= Игрок 1 =- тянет спички. Можно взять: "
#define STR_ACT_PL2T	"\n.::Игрок 2::. тянет спички. Можно взять: "
#define STR_ACT_AITN	"\nТеперь мой ход. Я тяну спички. Я могу взять: "
#define STR_ACT_AIPK	"Я взял %lu спичек\n"
#define STR_RES_WINR	"\nПобедитель! Игрок %u победил\n"
#define STR_RES_PWIN	"\nТВОЯ ВЗЯЛА! Тебе просто повезло.\nДавай ещё разок!\n"
#define STR_RES_AWIN	"\nХа! Я ВЫИГРАЛ!\nБесполезно пытаться победить машину!\n"
#define STR_INF_MTCH	"\n\nОсталось спичек: %lu\n"
#define STR_INF_ENDL	"неограничено\n"
#define STR_INF_ETRL	"%lu\n> "

#define STR_OMN_EXIT	"0 - выход (type '0' or 'exit')\n> "
#define STR_MMN_TITL	"\n\n    .::Главное меню::.\n"
#define STR_MMN_PVE0	"1 - игрок против машины\n"
#define STR_MMN_PVP0	"2 - игрок против игрока\n"
#define STR_MMN_PVE1	"3 - игрок против машины (в цикле)\n"
#define STR_MMN_PVP1	"4 - игрок против игрока (в цикле)\n"
#define STR_MMN_CFGM	"5 - настройки\n"
#define STR_CMN_TITL	"\n\n    .::Меню настроек::.\n"
#define STR_CMN_MAXM	"1 - максимум спичек = %lu (исходно = 50)\n"
#define STR_CMN_FSTS	"2 - первый ход = %lu спичек (исходно = 2)\n"
#define STR_CMN_FSTT	"3 - первым начинает = %u (0 - машина, 1 - игрок, исходно = 0)\n"
#define STR_CMN_DLVL	"4 - значение отладки = %u (0 - выкл., 1..3 - вкл., исходно = 0)\n"
#define STR_CMN_RSET	"5 - восстановить исходные значения\n"
#define STR_CMN_CFGI	"Ввод нового значения (0 or exit - для отмены):\n> "
#define STR_CMN_CFGR	"Все параметры восстановлены до исходных значений\n"

#define STR_DBG_IKEY	"Нажата клавиша = %u\n"
#define STR_DBG_FIBO	"Спичек макс. = %lu, Фибоначчи = %lu, fibsz = %lu\n"
#define STR_DBG_RAND	"Принято случайное значение: %lu\n"
#define STR_DBG_CALC	"Расчитано значение: %lu\n"
#define STR_DBG_ONCE	"Забираю всё разом...\n"
#define STR_DBG_INDX	"%lu: Фибоначчи: %lu, остаётся: %lu, предел: %lu\n"
#define STR_DBG_UFAV	"Неблагоприятный исход: %lu\n"
#define STR_DBG_STEP	"Благоприятный исход найден за один шаг\n"
#define STR_DBG_RDEP	"Глубина рекурсии = %lu\n"
#define STR_DBG_FAVR	"Благоприятный исход найден рекурсивно\n"
#define STR_DBG_PPRL	"Игрок %u: осталось спичек = %lu, предел = %lu\n"
#define STR_DBG_PDIS	"Игрок: %u\n"


volatile unsigned long bm_limit = 0, bm_remain = 0;
volatile unsigned char bm_player = 0, bm_debug = 0, bm_first_turn = 0;
volatile unsigned long bm_max_matches = 50, bm_first_step = 2;
volatile unsigned long bm_fibsz = 0, bm_recursion = 0;
unsigned long *bm_fibonacci = NULL, *bm_fibtmp;

void echo_matches( void );
void echo_player_pickup( unsigned char num );
void ai_pickup( void );
unsigned long ai_decide( unsigned long ai_remain, unsigned long ai_limit );
unsigned long player_input( void );
unsigned char player_pickup( unsigned char num );
void pvp_loop( unsigned char forever );
void pve_loop( unsigned char forever );
void configure( void );

int main( void ) {
	unsigned char keypress;
	/* Главное меню */
	do {
		printf( STR_MMN_TITL );
		printf( STR_MMN_PVE0 );
		printf( STR_MMN_PVP0 );
		printf( STR_MMN_PVE1 );
		printf( STR_MMN_PVP1 );
		printf( STR_MMN_CFGM );
		printf( STR_OMN_EXIT );

		keypress = ( unsigned char ) player_input();
		bm_limit = bm_first_step;

		bm_fibsz = 0;
		do {
			/* Динамический массив для последовательности Фибоначчи */
			bm_fibtmp = ( unsigned long * ) realloc( bm_fibonacci,
					( bm_fibsz + 1 ) * sizeof( unsigned long ));
			if ( bm_fibtmp == NULL) {
				printf( STR_ERR_MALL );
				keypress = 0;
				break;
			}
			bm_fibonacci = bm_fibtmp;
			bm_fibonacci[bm_fibsz] = ( int ) bm_rndcomput( bm_fibsz );
			if ( bm_debug )
				printf( "%lu: %lu @ %p\n", bm_fibsz,
						bm_fibonacci[bm_fibsz], bm_fibtmp );
			bm_fibsz ++;
		} while ( bm_fibonacci[bm_fibsz -1] <= bm_max_matches );
		bm_fibsz --;
		if ( bm_debug )
			printf( STR_DBG_FIBO,
					bm_max_matches, bm_fibonacci[bm_fibsz], bm_fibsz );

		switch ( keypress ) {
		case 1: pve_loop( 0 ); break;
		case 2: pvp_loop( 0 ); break;
		case 3: pve_loop( 1 ); break;
		case 4: pvp_loop( 1 ); break;
		case 5: configure(); break;
		default: break;
		}
		if ( bm_debug >= 3 ) printf( STR_DBG_IKEY, keypress );
	} while ( keypress > 0 );

	return ( 0 );
}

void echo_matches( void ) {
	unsigned long i = 0;
	printf( STR_INF_MTCH, bm_remain );
	/* Обработчик ошибок для беззнакового целого (мало ли что) */
	if ( bm_remain > bm_max_matches ) {
		printf( STR_ERR_UNKN );
		return;
	}
	for ( ; i < bm_remain; i ++ ) printf( "|" );
	printf( "\n" );

	return;
}

void echo_player_pickup( unsigned char num ) {
	/* Тескт сообщений для каждого игрока */
	switch ( num ) {
	case 0: printf( STR_ACT_PL0T ); break;
	case 1: printf( STR_ACT_PL1T ); break;
	case 2: printf( STR_ACT_PL2T ); break;
	default: printf( STR_ERR_WRPL ); break;
	}
	if ( bm_limit ) printf( STR_INF_ETRL, bm_limit );
	else printf( STR_INF_ENDL );

	return;
}

void ai_pickup( void ) {
	/* Обработка хода для машины */
	unsigned long bm_ai_take = 0;

	printf( STR_ACT_AITN );
	if ( bm_limit ) printf( "%lu\n", bm_limit );
	else printf( STR_INF_ENDL );

	/* Расчёт благоприятного исхода */
	bm_ai_take = ai_decide( bm_remain, bm_limit );
	if ( bm_ai_take == 0 ) {
		/* Неблагоприятный исход - брать случайным образом */
		srandom( time( NULL ) );
		bm_ai_take = random() % bm_limit + 1;
		if ( bm_debug ) printf( STR_DBG_RAND, bm_ai_take );
	} else if ( bm_debug ) printf( STR_DBG_CALC, bm_ai_take );

	/* Показать, сколько было взято спичек */
	printf( STR_ACT_AIPK, bm_ai_take );

	/* Изменение глобальных переменных соответств. ходу */
	bm_remain -= bm_ai_take;
	bm_limit = bm_ai_take * 2;

	return;
}

/* Рекурсивная функция для поиска благоприятного исхода */
unsigned long ai_decide( unsigned long ai_remain, unsigned long ai_limit ) {
	unsigned long bm_n = 0, bm_m = 0, bm_fav = 0;

	/* 1. Если можно взять всё сразу и победить, то взять */
	if ( ai_remain <= ai_limit ) {
		if ( bm_debug >= 2 ) printf( STR_DBG_ONCE );
		return ( ai_remain );
	}

	/* 2. Поиск числа Фибоначчи, меньшего, чем остаток */
	do bm_n ++;
	while( bm_fibonacci[bm_n] <= ai_remain &&
			bm_fibonacci[bm_n + 1] < ai_remain &&
			bm_n < bm_fibsz );
	if ( bm_debug >= 2 )
		printf( STR_DBG_INDX,
				bm_n, bm_fibonacci[bm_n], ai_remain, ai_limit );

	/* Если остаток - число Фибоначчи, то исход неблагоприятен */
	if ( ai_remain == bm_fibonacci[bm_n] ) {
		if ( bm_debug >= 2 ) printf( STR_DBG_UFAV, bm_fibonacci[bm_n] );
		return ( 0 );
	}

	/* 3. Если можно дать оппоненту число Фибоначчи, то дать */
	bm_fav = ai_remain - bm_fibonacci[bm_n];
	bm_m = ( int ) trunc( ( bm_fibonacci[bm_n] / 2 ) - 0.5 );
	if ( bm_fav <= ai_limit && bm_fav <= bm_m ) {
		if ( bm_debug >= 2 ) printf( STR_DBG_STEP );
		return ( bm_fav );
	}

	/* 4. Рекурсивный поиск хода, ведущего к благоприятному исходу */
	if ( bm_debug >= 2 ) {
		bm_recursion ++;
		printf( STR_DBG_RDEP, bm_recursion );
	}
	bm_m = bm_min(ai_limit, ( int ) trunc( ( ai_remain - 1 ) / 3 ) );
	for ( bm_fav = 1; bm_fav <= bm_m; bm_fav ++ ) {
		if ( ai_decide( ai_remain - bm_fav, bm_fav * 2 ) == 0 ) {
			if ( bm_debug >= 2 ) printf( STR_DBG_FAVR );
			return ( bm_fav );
		}
	}

	return ( 0 );
}

unsigned long player_input( void ) {
	unsigned long bm_input = 0;
	int bm_i = 0;
	char *bm_inbuf = ( char * ) malloc( 4 ), *bm_exit = "exit";
	/* Цикл обработки введённых значений */
	while ( ( bm_i = scanf( "%lu", &bm_input ) ) < 1 ) {
		scanf( "%4s", bm_inbuf );
		/* Условие выхода - слово 'exit' */
		if ( bm_inbuf[0] == bm_exit[0]
			&& bm_inbuf[1] == bm_exit[1]
			&& bm_inbuf[2] == bm_exit[2]
			&& bm_inbuf[3] == bm_exit[3] ) {
			return ( 0 );
		}
		/* Очистка буфера ввода и ругательство */
		scanf( "%*[^\n]" );
		printf( STR_ERR_INUM );
	}
	return ( bm_input );
}

unsigned char player_pickup( unsigned char num ) {
	unsigned long bm_player_take;
	do {
		echo_player_pickup( num );
		bm_player_take = player_input();
		/* Если игрок ввёл '0' или 'exit', то выход */
		if ( ! bm_player_take ) {
			bm_remain = 0;
			bm_limit = 1;
			return ( bm_player_take );
		}
	} while ( ( bm_player_take > bm_limit ) && bm_limit );

	/* Если можно взять хоть сколько (bm_limit == 0) */
	bm_remain -= ( ! bm_limit ) ? bm_remain : bm_player_take; 
	/* Предельно можно взять меньше не больше, чем осталось */
	bm_limit = ( ( bm_player_take * 2 ) <= bm_remain ) ?
			( bm_player_take * 2 ) : bm_remain;

	/* Только 3 типа игроков, иначе ругаться */
	switch ( num ) {
	case 1: break;
	case 2: break;
	case 0: break;
	default: printf( STR_ERR_UNKN ); break;
	}
	if ( bm_debug >= 3 )
		printf( STR_DBG_PPRL, num, bm_remain, bm_limit );

	return ( 1 );
}

void pvp_loop( unsigned char forever ) {
	do {
		bm_player = 1;
		bm_remain = bm_max_matches;
		bm_limit = bm_first_step;
		/* Игроки поочереди берут, пока не останется 0 спичек */
		while ( bm_remain > 0 ) {
			echo_matches();
			if ( ! player_pickup( bm_player ) ) forever = 0;
			if ( bm_limit ) bm_player = ( bm_player & 1 ) ? 2 : 1;
		}
		printf( STR_RES_WINR, bm_player );
	} while ( forever );

	return;
}

void pve_loop( unsigned char forever ) {
	do {
		bm_recursion = 0;
		bm_player = bm_first_turn;
		bm_remain = bm_max_matches;
		bm_limit = bm_first_step;
		/* Игрок и машина поочереди берут, пока есть спички */
		while ( bm_remain > 0 ) {
			echo_matches();
			if ( bm_debug ) printf( STR_DBG_PDIS, bm_player );
			if ( bm_player ) {
				if ( ! player_pickup( 0 ) ) bm_player = forever = 0;
			}
			else ai_pickup();
			if ( ! bm_remain ) break;
			bm_player ++;
			bm_player = bm_player & 0x01;
		}
		if ( bm_player ) printf( STR_RES_PWIN );
		else printf( STR_RES_AWIN );
	} while ( forever );

	return;
}

void configure( void ) {
	/* Меню настроек */
	unsigned char bm_keypress;
	unsigned long bm_value = 0;
	do {
		printf( STR_CMN_TITL );
		printf( STR_CMN_MAXM, bm_max_matches );
		printf( STR_CMN_FSTS, bm_first_step );
		printf( STR_CMN_FSTT, bm_first_turn );
		printf( STR_CMN_DLVL, bm_debug );
		printf( STR_CMN_RSET );
		printf( STR_OMN_EXIT );
		/* Get input and process */
		bm_keypress = ( unsigned char ) player_input();
		switch ( bm_keypress ) {
		case 1:
			printf( STR_CMN_CFGI );
			bm_value = player_input();
			if ( bm_value ) bm_max_matches = bm_value;
			break;
		case 2:
			printf( STR_CMN_CFGI );
			bm_value = player_input();
			if ( bm_value ) bm_first_step = bm_value;
			break;
		case 3:
			printf( STR_CMN_CFGI );
			bm_value = player_input();
			if ( bm_value ) bm_first_turn = 1;
			else bm_first_turn = 0;
			break;
		case 4:
			printf( STR_CMN_CFGI );
			bm_value = player_input();
			if ( bm_value ) bm_debug = bm_value % 4;
			break;
		case 5:
			bm_max_matches = 50;
			bm_first_step = 2;
			bm_first_turn = 0;
			bm_debug = 0;
			printf( STR_CMN_CFGR );
			break;
		default: break;
		}
		if ( bm_debug >= 3 ) printf( STR_DBG_IKEY, bm_keypress );
	} while ( bm_keypress > 0 );

	return;
}
