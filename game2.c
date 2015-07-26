#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>

#define MSPF (100/60)
#define NORMAL 0
#define BOSS 1
#define BATTLE 2
#define BOSSDEAD 3
#define CLEAR 4
#define STAGEGAMEN 5
#define MAXHP 5
#define BOSSMAXHP 10
#define BEAMSUU 20
#define ENEMYSUU 10
#define ITEMSUU 2

void setting();
void DISPLAY();
void run();
void appearBeam(int a_y,int a_x);
void makeENEMY_BEAM();
void makeBOSS_BEAM();
void moveEnemy_BEAM();
void moveBoss_BEAM();
void moveBeam();
void makeEnemy();
void makeBoss();
void moveEnemy();
void moveBoss();
void hantei();
void bshantei();
void imhantei(int a_y,int a_x);
int atari(int bm_y,int bm_x,int ene_y,int ene_x,int lev,char *form);
void HP(int hp);
void BOSS_HP(int bshp);
void en_attack(int a_y,int a_x,int joutai);
void en_bm_attack(int a_y,int a_x,int joutai);
void bs_attack(int a_y,int a_x,int joutai);
void makeITEM();
void moveITEM();
void score_keisan(int enemyscore);


int kaisuu=0;
int cnt=0,cnt1=0,cnt2=0,cnt3=0,cnt4=0;
int hp = MAXHP;
int GO = 0;
int pattern = 0;
int pat2 = 1;
int field = STAGEGAMEN;
int temphp;
int temp=0;
int message = 0;
int m = 0;
int dam = 0;
int exitbs = 0;
int deathcnt = 0;
int d_x1,d_x2;
int bossidou=0;
int ALLENEMY=0;
int critical = 0;

struct ME{
	int joutai;
	char me_form[4];
	int a_y;
	int a_x;
	int stage;
	int score;
} me[1];

struct Beam{
	int beamexsist;//0 or 1:0 is not exsist,1 is exsist.
	int beam_y;
	int beam_x;
	int beam_type;
	/* struct Beam *nextp; */
} bn[BEAMSUU];

struct Enemy{
	int level;
	char *enemy_form;
	int enemyexsist;
	int enemy_y;
	int enemy_x;
	int enemy_hp;
	int enjoutai;
	int enscore;
	struct Beam enbm[10];
} en[ENEMYSUU];

struct BOSS_ENEMY{
	int blevel;
	int bossexsist;
	int boss_y;
	int boss_x;
	int boss_hp;
	int bsscore;
	struct Beam bsbm[10];
}bs[3];

struct ITEM{
	int number;
	char item_form[2];
	int itemexsist;
	int item_y;
	int item_x;
}im[ITEMSUU];

int main()
{
	//初期化
	initscr();
	//設定
	setting();
	//ゲーム実行
	DISPLAY();
	run();

	endwin();
	return 0;
}

void setting()
{
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr,TRUE);
	timeout(0);
}

void DISPLAY(){
	mvaddstr(0,0,"**************************************");
	mvaddstr(1,0,"*                        *           *");
	mvaddstr(2,0,"*                        *           *");
	mvaddstr(3,0,"*                        *           *");
	mvaddstr(4,0,"*                        *           *");
	mvaddstr(5,0,"*                        *************");
	mvaddstr(6,0,"*                        *           *");
	mvaddstr(7,0,"*                        *           *");
	mvaddstr(8,0,"*                        *           *");
	mvaddstr(9,0,"*                        *************");
	mvaddstr(10,0,"*                        *           *");
	mvaddstr(11,0,"*                        *           *");
	mvaddstr(12,0,"*                        *           *");
	mvaddstr(13,0,"*                        *           *");
	mvaddstr(14,0,"*                        *************");
	mvaddstr(15,0,"*                        *           *");
	mvaddstr(16,0,"**************************************");
}


void run()
{
	
	clock_t time1,pre_time;
	char ch;
	char clearmes[15];
	int beamdelay = 0;
	int beam = 0;//beamが1のときに画面にビームがあります
	/* int beam_x,beam_y; */
	int delay,delay_s;
	int delay_st = 1;
	int kotei_delay1 = 0;
	int kotei_delay2 = 0;
	int kotei_delay3 = 0;
	int wait = 9000;
	me[0].joutai = 0;
	me[0].a_x = 10;
	me[0].a_y = 15;
	me[0].stage = 1;
	me[0].score = 0;
	int enemy = 1;
	/* int enemy_x = 9; */
	/* int enemy_y = 3; */
	int randam = 0;
	int reala_x;
	int i;
	int smes = 0;
	int quit = 0;
	char stagemes[10];

	while((ch = getch()) != 's'){
		mvaddstr(7,7,"INVADAR GAME");
		mvaddstr(15,18,"q:exit");
		if(ch == 'q'){
			quit = 1;
			break;
		}
		if(ch == '2'){
			ALLENEMY = 31;
			me[0].stage = 2;
			mvaddstr(10,6,"jump to STAGE2");
		}
		if(ch == 'c'){
			ALLENEMY = 0;
			me[0].stage = 1;
			mvaddstr(10,6,"              ");
		}
		if((delay_s % 27000) == 0){
			if(smes == 0){
				mvaddstr(8,6,"-PRESS 's' KEY-");
				smes = 1;
			}else if(smes == 1){
				mvaddstr(8,6,"               ");
				smes = 0;
			}
		}
		delay_s++;
	}


	if(quit == 0){
		erase();
		DISPLAY();
		HP(hp);
		score_keisan(0);
		pre_time = 0;
		while((ch=getch()) != 'q'){
			time1 = clock();
			switch(ch){
				case 'f':
					if(me[0].joutai == 0){
						if(me[0].a_x > 1){
							mvaddch(me[0].a_y,me[0].a_x,' ');
							me[0].a_x--;
						}
					}else if(me[0].joutai == 1){
						if(me[0].a_x > 2){
							mvaddstr(me[0].a_y,me[0].a_x-1,"   ");
							me[0].a_x--;
						}
					}
					break;
				case 'j':
					if(me[0].joutai == 0){
						if(me[0].a_x < 24){
							mvaddch(me[0].a_y,me[0].a_x,' ');
							me[0].a_x++;
						}
					}else if(me[0].joutai == 1){
						if(me[0].a_x < 23){
							mvaddstr(me[0].a_y,me[0].a_x-1,"   ");
							me[0].a_x++;
						}
					}
					break;
				case ' ':
					if(bossidou == 0){
						appearBeam(me[0].a_y,me[0].a_x);
					}
					/* if(beam == 0){ */
					/* beam_x = a_x; */
					/* beam_y = a_y; */
					/* beep(); */
					/* beam = 1; */
					/* } */
					break;
			}
			/* if(beam == 1 && (delay % wait) == 0){ */
			/* mvaddch(beam_y,beam_x,' '); */
			/* beam_y--; */
			/* mvaddch(beam_y,beam_x,'i'); */
			/* if(beam_y < 0){ */
			/* mvaddch(beam_y,beam_x,' '); */
			/* beam = 0; */
			/* } */
			/* } */
			if(field == NORMAL){
				if((double)(time1 - pre_time) >= MSPF){
					if((delay % wait) == 0){
						moveEnemy();
						moveEnemy_BEAM();
						moveBeam();
						if(GO == 0){	
							hantei();
							en_bm_attack(me[0].a_y,me[0].a_x,me[0].joutai);
						}
					}
				}
			}else if(field == BATTLE){
				if((delay % wait) == 0){
					moveBoss();
					moveBeam();
					moveBoss_BEAM();
					if(bossidou == 0){
						if(GO == 0){
							bshantei();
							bs_attack(me[0].a_y,me[0].a_x,me[0].joutai);
						}
					}
				}
				if(bossidou == 0){
					if((delay % 27000) == 0){
						makeBOSS_BEAM();
					}
				}
			}
			if(GO == 1){
				mvaddstr(8,8,"GAME OVER");
			}
			if(field == NORMAL){
				if((delay % 45000) == 0){
					makeEnemy();
				}
			}else if(field == BOSS){
				erase();
				DISPLAY();
				HP(hp);
				score_keisan(0);
				makeBoss();
				BOSS_HP(bs[0].boss_hp);
				field = BATTLE;
			}
			if(GO == 0){
				if(field == NORMAL){
					if((delay % wait) == 0){
						moveITEM();
						srand((unsigned)time(NULL));
						randam = rand()%10;
						if(randam == 0){
							makeITEM();
						}
						imhantei(me[0].a_y,me[0].a_x);
					}
					if(message == 1){
						kotei_delay1 = delay;
						message = 0;
					}
					if((delay % (kotei_delay1+90000)) == 0){
						mvaddstr(11,27,"         ");
						mvaddstr(12,26,"           ");
					}
				}
			}
			if(field == BOSSDEAD){
				if((delay % 27000) == 0){
					moveBoss();
				}
			}
			if(exitbs == 1){
				for(i = 0; i< 2; i++){
					im[i].itemexsist = 0;
				}
				for(i = 0; i< BEAMSUU; i++){
					bn[i].beamexsist = 0;
				}
				for(i = 0; i < ENEMYSUU; i++){
					en[i].enemyexsist = 0;
				}
				kotei_delay1 = delay;
				exitbs = 0;
			}
			if(field == CLEAR){
				sprintf(clearmes,"-STAGE%d CLEAR-",me[0].stage-1);
				mvaddstr(8,6,clearmes);
				if((delay % (kotei_delay1+120000)) == 0){
					mvaddstr(8,6,"              ");
					field = STAGEGAMEN;
					kotei_delay3 = delay;
				}
			}
			if(field == STAGEGAMEN){
				sprintf(stagemes,"-STAGE%d-",me[0].stage);
				mvaddstr(8,9,stagemes);
				if(((delay+1) % (kotei_delay3+90000)) == 0){
					mvaddstr(8,9,"        ");
					field = NORMAL;
				}
			}


			/* if(enemy == 1 && (delay % wait) == 0){ */
			/* srand((unsigned)time(NULL)); */
			/* randam = rand()%10; */
			/* if((randam >= 0)&&(randam < 5)){ */
			/* mvaddstr(enemy_y,enemy_x,"   "); */
			/* enemy_x++; */
			/* enemy_x--; */
			/* } */
			/* } */
			/* if(beam_y == enemy_y && (beam_x == enemy_x || beam_x == enemy_x+1 || beam_x == enemy_x+2)){ */
			/* mvaddstr(enemy_y,enemy_x,"   "); */
			/* beam = 0; */
			/* enemy = 0; */
			/* mvaddstr(12,3,"HIT!"); */
			/* } */
			/* if(enemy == 1) mvaddstr(enemy_y,enemy_x,"orz"); */
			if(dam == 1){
				mvaddstr(me[0].a_y,me[0].a_x-1,"   ");
				dam = 0;
			}
			switch(me[0].joutai){
				case 0:
					sprintf(me[0].me_form,"A");
					reala_x = me[0].a_x;
					break;
				case 1:
					sprintf(me[0].me_form,"AAA");
					reala_x = me[0].a_x-1;
					break;
			}
			mvaddstr(me[0].a_y,reala_x,me[0].me_form);
			move(LINES-1,COLS-1);//カーソルが邪魔なので移動
			delay++;
			pre_time = time1;
		}
	}
}

void appearBeam(int a_y,int a_x){
	
	if(me[0].joutai == 0){	
		if(cnt < BEAMSUU){
			if(bn[cnt].beamexsist == 0){
				bn[cnt].beamexsist = 1;
				bn[cnt].beam_y = a_y;
				bn[cnt].beam_x = a_x;
			}
		}
		if(bn[0].beamexsist == 0){
			if(cnt >= BEAMSUU-1) cnt = 0;
		}
		cnt++;
	}else if(me[0].joutai == 1){
		if(cnt < BEAMSUU-2){
			if(bn[cnt].beamexsist == 0 && bn[cnt+1].beamexsist == 0 && bn[cnt+2].beamexsist == 0){
				bn[cnt].beamexsist = 1;
				bn[cnt+1].beamexsist = 1;
				bn[cnt+2].beamexsist = 1;
				bn[cnt].beam_y = a_y;
				bn[cnt+1].beam_y = a_y;
				bn[cnt+2].beam_y = a_y;
				bn[cnt].beam_x = a_x-1;
				bn[cnt+1].beam_x = a_x;
				bn[cnt+2].beam_x = a_x+1;
			}
		}
		if(bn[0].beamexsist == 0){
			if(cnt >= BEAMSUU-3) cnt = 0;
		}
		cnt += 3;
	}


	/* struct Beam *p; */
	/* p =(struct Beam *)malloc(sizeof(struct Beam)); */
	
	/* p->beamexsist = 1; */
	/* p->beam_y = a_y; */
	/* p->beam_x = a_x; */
	
	/* p->nextp = root; */
	/* root = p; */
}

void makeENEMY_BEAM(){
	int i;
	for(i = 0; i < ENEMYSUU; i++){
		if(en[i].enemyexsist == 1){
			en[i].enbm[cnt4].beamexsist = 1;
			en[i].enbm[cnt4].beam_y = en[i].enemy_y;
			en[i].enbm[cnt4].beam_x = en[i].enemy_x;
			en[i].enbm[cnt4].beam_type = 1;
			en[i].enbm[cnt4+1].beamexsist = 1;
			en[i].enbm[cnt4+1].beam_y = en[i].enemy_y;
			en[i].enbm[cnt4+1].beam_x = en[i].enemy_x;
			en[i].enbm[cnt4+1].beam_type = 2;
			en[i].enbm[cnt4+2].beamexsist = 1;
			en[i].enbm[cnt4+2].beam_y = en[i].enemy_y;
			en[i].enbm[cnt4+2].beam_x = en[i].enemy_x;
			en[i].enbm[cnt4+2].beam_type = 3;
			cnt4++;
		}
	}
	if(cnt4 > 7){
		cnt4 = 0;
	}
}

void makeBOSS_BEAM(){
	
	bs[0].bsbm[cnt1].beamexsist = 1;
	bs[0].bsbm[cnt1].beam_y = bs[0].boss_y+1;
	bs[0].bsbm[cnt1].beam_x = bs[0].boss_x+1;
	cnt1++;
	if(cnt1 >= 10){
		cnt1 = 0;
	}
}

void moveEnemy_BEAM(){
	int i,j;
	for(i = 0; i < ENEMYSUU; i++){
		for(j = 0; j < 10; j++){
			if(en[i].enbm[j].beamexsist == 1){
				mvaddch(en[i].enbm[j].beam_y,en[i].enbm[j].beam_x,' ');
				switch(en[i].enbm[j].beam_type){
					case 1:
						en[i].enbm[j].beam_y++;
						break;
					case 2:
						if(en[i].enbm[j].beam_x > 1){
							if((en[i].enbm[j].beam_y % 2) == 0){
								en[i].enbm[j].beam_x--;
							}
						}
						en[i].enbm[j].beam_y++;
						break;
					case 3:
						if(en[i].enbm[j].beam_x < 24){
							if((en[i].enbm[j].beam_y % 2) == 0){
								en[i].enbm[j].beam_x++;
							}
						}
						en[i].enbm[j].beam_y++;
						break;
				}
				mvaddch(en[i].enbm[j].beam_y,en[i].enbm[j].beam_x,'V');
				if(en[i].enbm[j].beam_y >= me[0].a_y+1){
					mvaddch(en[i].enbm[j].beam_y,en[i].enbm[j].beam_x,'*');
					en[i].enbm[j].beamexsist = 0;
				}
			}
		}
	}
}

void moveBoss_BEAM(){
	int i;
	for(i = 0; i < 10; i++){
		if(bs[0].bsbm[i].beamexsist == 1){
			mvaddch(bs[0].bsbm[i].beam_y,bs[0].bsbm[i].beam_x,' ');
			bs[0].bsbm[i].beam_y++;
			mvaddch(bs[0].bsbm[i].beam_y,bs[0].bsbm[i].beam_x,'V');
			if(bs[0].bsbm[i].beam_y >= me[0].a_y+1){
				mvaddch(bs[0].bsbm[i].beam_y,bs[0].bsbm[i].beam_x,'*');
				bs[0].bsbm[i].beamexsist = 0;
			}
		}
	}
}
	

void moveBeam(){
	int i;
	for(i = 0;i < BEAMSUU; i++){
		if(bn[i].beamexsist == 1){
			mvaddch(bn[i].beam_y,bn[i].beam_x,' ');
			bn[i].beam_y--;
			mvaddch(bn[i].beam_y,bn[i].beam_x,'i');
			if(bn[i].beam_y < 1){
				mvaddch(bn[i].beam_y,bn[i].beam_x,'*');
				bn[i].beamexsist = 0;
			}
		}
	}

	/* struct Beam *p; */

	/* for(p = root; p != NULL; p = p->nextp){ */
		/* if(p->beamexsist == 1){ */
			/* mvaddch(p->beam_y,p->beam_x,' '); */
			/* p->beam_y--; */
			/* mvaddch(p->beam_y,p->beam_x,'i'); */
			/* if(p->beam_y < 0){ */
				/* mvaddch(p->beam_y,p->beam_x,' '); */
				/* p->beamexsist = 0; */
			/* } */
		/* } */
	/* } */
}

void makeEnemy(){
	int randam1;

	srand((unsigned)time(NULL));
	randam1 = rand()%18;
	if(ALLENEMY<21){
		en[cnt2].level = 1;
		en[cnt2].enemyexsist = 1;
		en[cnt2].enemy_y = 1;
		en[cnt2].enemy_x = randam1+2;
		en[cnt2].enemy_hp = 1;
		en[cnt2].enjoutai = 0;
	}else if(ALLENEMY >= 21 && ALLENEMY < 30){
		en[cnt2].level = 2;
		en[cnt2].enemyexsist = 1;
		en[cnt2].enemy_y = 1;
		en[cnt2].enemy_x = randam1+2;
		en[cnt2].enemy_hp = 2;
		en[cnt2].enjoutai = 0;
	}else if(ALLENEMY >= 30){
		if(me[0].stage == 1){
			field = BOSS;
		}else if(me[0].stage == 2){
			if(ALLENEMY >=31 && ALLENEMY < 50){
				en[cnt2].level = 3;
				en[cnt2].enemyexsist = 1;
				en[cnt2].enemy_y = 1;
				en[cnt2].enemy_x = randam1+2;
				en[cnt2].enemy_hp = 1;
				en[cnt2].enjoutai = 0;
			}else if(ALLENEMY >= 50){
				en[cnt2].level = 4;
				en[cnt2].enemyexsist = 1;
				en[cnt2].enemy_y = 1;
				en[cnt2].enemy_x = randam1+2;
				en[cnt2].enemy_hp = 3;
				en[cnt2].enjoutai = 0;
			}
		}
	}
	if(field == NORMAL){
		switch(en[cnt2].level){
			case 1:
				en[cnt2].enemy_form = (char *)malloc(strlen("orz")+1);
				sprintf(en[cnt2].enemy_form,"orz");
				en[cnt2].enscore = 10;
				break;
			case 2:
				en[cnt2].enemy_form = (char *)malloc(strlen("69")+1);
				sprintf(en[cnt2].enemy_form,"69");
				en[cnt2].enscore = 30;
				break;
			case 3:
				en[cnt2].enemy_form = (char *)malloc(strlen("BM")+1);
				sprintf(en[cnt2].enemy_form,"BM");
				en[cnt2].enscore = 50;
				break;
			case 4:
				en[cnt2].enemy_form = (char *)malloc(strlen("T<o>T")+1);
				sprintf(en[cnt2].enemy_form,"T<o>T");
				en[cnt2].enscore = 80;
				break;
		}
		mvaddstr(en[cnt2].enemy_y,en[cnt2].enemy_x,en[cnt2].enemy_form);
		cnt2++;
	}
	if(cnt2 > ENEMYSUU-1) cnt2 = 0;
	ALLENEMY++;
}

void makeBoss(){
		bs[0].blevel = 1;
		bs[0].bossexsist = 1;
		bs[0].boss_y = 1;
		bs[0].boss_x = 10;
		bs[0].boss_hp = 10;
		bs[0].bsscore = 100;
		temphp = bs[0].boss_hp;
		mvaddstr(bs[0].boss_y,bs[0].boss_x,"p--q");
		mvaddstr(bs[0].boss_y+1,bs[0].boss_x,"B**O");
}

void moveEnemy(){
	int i,j;
	if(pattern == 1){
		pattern--;
		pat2 = 1;
	}else if(pattern == -1){
		pattern++;
		pat2 = -1;
	}else if(pattern == 0){
		if(pat2 == 1){
			pattern--;
		}else if(pat2 == -1){
			pattern++;
		}
	}
	for(i = 0; i < ENEMYSUU; i++){
		if(en[i].enemyexsist == 1){
			for(j = 0; j < strlen(en[i].enemy_form); j++) mvaddch(en[i].enemy_y,en[i].enemy_x+j,' ');
			switch(en[i].level){
				case 1:
				case 2:
				case 4:
					en[i].enemy_y++;
					en[i].enemy_x += pattern;
					break;
				case 3:
					if(en[i].enemy_y != 4 && en[i].enjoutai == 0){
						en[i].enemy_y++;
					}else if(en[i].enemy_y == 4 && en[i].enjoutai == 0){
						makeENEMY_BEAM();
						en[i].enjoutai = 1;
					}else if(en[i].enemy_y == 4 && en[i].enjoutai == 1){
						en[i].enemy_y--;
					}else if(en[i].enemy_y != 4 && en[i].enjoutai == 1){
						en[i].enemy_y--;
					}
					break;
			}
			mvaddstr(en[i].enemy_y,en[i].enemy_x,en[i].enemy_form);
			switch(en[i].level){
				case 1:
				case 2:
				case 4:
					if(en[i].enemy_y == me[0].a_y){
					en_attack(me[0].a_y,me[0].a_x,me[0].joutai);
					for(j = 0; j < strlen(en[i].enemy_form); j++) mvaddch(en[i].enemy_y,en[i].enemy_x+j,' ');
					en[i].enemyexsist = 0;
					}
					break;
				case 3:
					if(en[i].enemy_y == 0){
					en[i].enjoutai = 0;
					for(j = 0; j < strlen(en[i].enemy_form); j++) mvaddch(en[i].enemy_y,en[i].enemy_x+j,'*');
					en[i].enemyexsist = 0;
					}
			}
		}
	}
}

void moveBoss(){
	int randam2;
	srand((unsigned)time(NULL));
	randam2 = rand()%20;
	
	if(bs[0].bossexsist == 1){
		if(field == BATTLE){
			if(bs[0].boss_y != 6){
				bossidou = 1;
				if((bs[0].boss_y%2)==1){
					mvaddstr(8,6,"-BOSS BATTLE-");
					m = 1;
				}else{
					mvaddstr(8,6,"             ");
					m = 0;
				}
				mvaddstr(bs[0].boss_y,bs[0].boss_x,"    ");
				mvaddstr(bs[0].boss_y+1,bs[0].boss_x,"    ");
				bs[0].boss_y++;
				mvaddstr(bs[0].boss_y,bs[0].boss_x,"p--q");
				mvaddstr(bs[0].boss_y+1,bs[0].boss_x,"B**O");
			}else{
				bossidou = 0;
				if(m == 1) mvaddstr(8,6,"             ");
				mvaddstr(bs[0].boss_y,bs[0].boss_x,"    ");
				mvaddstr(bs[0].boss_y+1,bs[0].boss_x,"    ");

				if(bs[0].boss_x <= 1){
					bs[0].boss_x++;
				}else if(bs[0].boss_x >= 21){
					bs[0].boss_x--;
				}else{
					if(randam2 >= 0 && randam2 < 10){
						bs[0].boss_x++;
					}else{
						bs[0].boss_x--;
					}
				}
				mvaddstr(bs[0].boss_y,bs[0].boss_x,"p--q");
				mvaddstr(bs[0].boss_y+1,bs[0].boss_x,"B**O");
				m = 0;
			}

		}else if(field == BOSSDEAD){
			deathcnt++;
			if(deathcnt == 1){
				mvaddstr(bs[0].boss_y,bs[0].boss_x,"p--q");
				mvaddstr(bs[0].boss_y+1,bs[0].boss_x,"B**O");
				d_x1 = bs[0].boss_x;
				d_x2 = bs[0].boss_x+2;
			}else if(deathcnt == 5){
				mvaddstr(bs[0].boss_y,d_x1,"  ");
				mvaddstr(bs[0].boss_y,d_x2,"  ");
				mvaddstr(bs[0].boss_y+1,d_x1,"  ");
				mvaddstr(bs[0].boss_y+1,d_x2,"  ");
				bs[0].bossexsist = 0;
				field = CLEAR;
				exitbs = 1;
				deathcnt = 0;
			}else{
			mvaddstr(bs[0].boss_y,d_x1,"  ");
			mvaddstr(bs[0].boss_y,d_x2,"  ");
			mvaddstr(bs[0].boss_y+1,d_x1,"  ");
			mvaddstr(bs[0].boss_y+1,d_x2,"  ");
			d_x1 = d_x1-1;
			d_x2 = d_x2+1;
			mvaddstr(bs[0].boss_y,d_x1,"p-");
			mvaddstr(bs[0].boss_y,d_x2,"-q");
			mvaddstr(bs[0].boss_y+1,d_x1,"B*");
			mvaddstr(bs[0].boss_y+1,d_x2,"*O");
			}
		}
	}
}


void hantei(){
	int i,j,k;
	char str[10];
	for(i = 0; i < BEAMSUU ; i++){
		for(j = 0; j <ENEMYSUU ; j++){
			if(bn[i].beamexsist == 1 && en[j].enemyexsist == 1){
				if(atari(bn[i].beam_y,bn[i].beam_x,en[j].enemy_y,en[j].enemy_x,en[j].level,en[j].enemy_form) == 1){
					en[j].enemy_hp--;
					if(critical == 1){
						en[j].enemy_hp -= 2;
						critical = 0;
					}
					if(en[j].enemy_hp == 0){
					kaisuu++;
					sprintf(str,"KILL:%2d",kaisuu);
					mvaddstr(10,27,"      ");
					mvaddstr(10,27,str);
					score_keisan(en[j].enscore);
					for(k = 0; k < strlen(en[j].enemy_form); k++) mvaddch(en[j].enemy_y,en[j].enemy_x+k,' ');
					en[j].enemyexsist = 0;
					}
					mvaddch(bn[i].beam_y,bn[i].beam_x,' ');
					bn[i].beamexsist = 0;
				}
			}
		}
	}
}

void bshantei(){
	int i;
	for(i = 0; i < BEAMSUU; i++){
		if(bn[i].beamexsist == 1 && bs[0].bossexsist == 1){
			if(atari(bn[i].beam_y,bn[i].beam_x,bs[0].boss_y,bs[0].boss_x,bs[0].blevel,NULL) == 1){
				temphp--;
				BOSS_HP(temphp);
				mvaddch(bn[i].beam_y,bn[i].beam_x,' ');
				bn[i].beamexsist = 0;
			}
		}
	}
}

void imhantei(int a_y,int a_x){
	int i;
	for(i = 0; i < ITEMSUU; i++){
		if(im[i].itemexsist == 1){
			if(im[i].item_y == a_y && im[i].item_x == a_x){
				mvaddstr(11,27,"ITEM GET!");
				message = 1;
				switch(im[i].number){
					case 1:
						me[0].joutai = 1;
						mvaddstr(12,27,"POWER UP!");
						break;
					case 2:
						if(hp < MAXHP){
							hp++;
							HP(hp);
							mvaddstr(12,27,"RECOVRY!");
						}else{
							mvaddstr(12,26,"ALREADY MAX");
							}
						
						break;
				}
			}
		}
	}
}

int atari(int bm_y,int bm_x,int ene_y,int ene_x,int lev,char *form){
	int i;
	if(field == NORMAL){
		for(i = 0; i < strlen(form); i++){
			if((bm_y == ene_y || bm_y == ene_y-1 ) && (bm_x == ene_x+i)){
				if(lev == 4 && i == 2) critical = 1;
				return(1);
			}
		}
		return(0);
	}else if(field == BATTLE){
		if((bm_y == ene_y || bm_y == ene_y+1) && (bm_x == ene_x || bm_x == ene_x+1 || bm_x == ene_x+2 || bm_x == ene_x+3)){
			return(1);
		}else{
			return(0);
		}
	}else{
		return(0);
	}
}

void en_attack(int a_y,int a_x,int joutai){
	int i,j,k;
	for(i = 0; i < ENEMYSUU; i++){
		if(en[i].enemyexsist == 1){
			for(j = 0; j < strlen(en[i].enemy_form); j++){
				for(k = 0; k < strlen(me[0].me_form); k++){
					if((en[i].enemy_y == a_y) && (en[i].enemy_x+j == a_x+k)){;
						if(joutai == 1){
							me[0].joutai = 0;
							dam = 1;
						}else{
							hp--;
							HP(hp);
						}
					}
				}
			}
		}
	}
}

void en_bm_attack(int a_y,int a_x, int joutai){
	int i,j;
	for(i = 0; i < ENEMYSUU; i++){
		for(j = 0; j < 10; j++){
			if(en[i].enbm[j].beamexsist == 1){
				if(joutai == 0){
					if(en[i].enbm[j].beam_y == a_y && en[i].enbm[j].beam_x == a_x){
						hp--;
						HP(hp);
					}
				}else if(joutai == 1){
					if((en[i].enbm[j].beam_y == a_y) && (en[i].enbm[j].beam_x == a_x-1 || en[i].enbm[j].beam_x == a_x || en[i].enbm[j].beam_x == a_x+1)){
						me[0].joutai = 0;
						dam = 1;
					}
				}
			}
		}
	}
}

void bs_attack(int a_y,int a_x,int joutai){
	int i;
	for(i = 0; i < 10; i++){
		if(bs[0].bsbm[i].beamexsist == 1){
			if(joutai == 0){
				if(bs[0].bsbm[i].beam_y == a_y && bs[0].bsbm[i].beam_x == a_x){
					hp--;
					HP(hp);
				}
			}else if(joutai == 1){
				if((bs[0].bsbm[i].beam_y == a_y) && (bs[0].bsbm[i].beam_x == a_x-1 || bs[0].bsbm[i].beam_x == a_x || bs[0].bsbm[i].beam_x == a_x+1)){
					me[0].joutai = 0;
					dam = 1;
				}
			}
		}
	}
}

void HP(int hp){
	int i;
	mvaddstr(15,27,"HP:");
	for(i = 0; i < MAXHP; i++){
		mvaddch(15,30+i,' ');
	}
	for(i = 0; i < hp; i++){
		mvaddch(15,30+i,'|');
	}
	if(hp == 0){
		GO = 1;
	}
}

void BOSS_HP(int bshp){
	int i;
	mvaddstr(1,27,"BOSS:");
	for(i = 0; i < bs[0].boss_hp ; i++){
		mvaddch(2,27+i,' ');
	}
	for(i = 0; i < bshp; i++){
		mvaddch(2,27+i,'|');
	}
	if(bshp == 0){
		erase();
		DISPLAY();
		HP(hp);
		score_keisan(bs[0].bsscore);
		me[0].stage = 2;
		field = BOSSDEAD;
		kaisuu++;
	}
}

void makeITEM(){
	int randam3,randam4;
	srand((unsigned)time(NULL));
	randam3 = rand()%23;
	randam4 = rand()%3;

	if(cnt3 < ITEMSUU){
		switch(randam4){
			case 0:
				im[cnt3].number = 1;
				break;
			case 1:
			case 2:
				im[cnt3].number = 2;
				break;
		}
		im[cnt3].itemexsist = 1;
		im[cnt3].item_y = 1;
		im[cnt3].item_x = randam3+1;

		switch(im[cnt3].number){
			case 1:
				sprintf(im[cnt3].item_form,"P");
				break;
			case 2:
				sprintf(im[cnt3].item_form,"E");
				break;
		}

		mvaddstr(im[cnt3].item_y,im[cnt3].item_x,im[cnt3].item_form);
		if(cnt3 == ITEMSUU-1) cnt3 = 0;
	}
	cnt3++;
}

void moveITEM(){
	int i;
	for(i = 0; i < ITEMSUU; i++){
		if(im[i].itemexsist == 1){
			mvaddch(im[i].item_y,im[i].item_x,' ');
			im[i].item_y++;
			mvaddstr(im[i].item_y,im[i].item_x,im[i].item_form);
			if(im[i].item_y == me[0].a_y+1){
				mvaddstr(im[i].item_y,im[i].item_x,"*");
				im[i].itemexsist = 0;
			}
		}
	}
}

void score_keisan(int enemyscore){
	char str[20];
	mvaddstr(6,26,"           ");
	mvaddstr(7,26,"           ");
	me[0].score += enemyscore;
	sprintf(str,"%10d",me[0].score);
	mvaddstr(6,26,"TOTAL SCORE");
	mvaddstr(7,26,str);
}

