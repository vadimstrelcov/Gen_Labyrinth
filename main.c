#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

char is_correct(char **map, int MAX_Y, int MAX_X) {
    typedef struct Point {
        int T;
        char status;
    } Point;
    Point **Map;
    int i,j;
    Map=(Point **)malloc(MAX_Y*sizeof(Point *));
    for (i=0;i<MAX_Y;i++) {
        Map[i]=(Point *)malloc(MAX_X*sizeof(Point));
    }
    for (i=0;i<MAX_Y;i++) {
        for (j=0;j<MAX_X;j++) {
            Map[i][j].T=-1;
            if (map[i][j]=='#') Map[i][j].status=0;
            if (map[i][j]=='.') Map[i][j].status=1;
            if (map[i][j]>='0' && map[i][j]<='9') Map[i][j].status=2;
            if (map[i][j]=='*') Map[i][j].status=3;
            if (map[i][j]=='@') Map[i][j].status=4;
        }
    }

    //algo
    int *newList=(int *)malloc(MAX_Y*MAX_X*sizeof(int));
    int *oldList=(int *)malloc(MAX_Y*MAX_X*sizeof(int));
    int coldlist=0,indexoldlist=0,indexnewlist=0;
    int T=0;
    int X=-1,Y=-1;
    for (i=0;i<MAX_Y;i++) {
        for (j=0;j<MAX_X;j++) {
            if (map[i][j]=='.') {
                X=j;
                Y=i;
                break;
            }
        }
        if (X!=-1) break;
		}
    Map[Y][X].T=0;
    oldList[coldlist++]=MAX_X*Y+X;
    while (1) {
        if (coldlist==0) break;
        indexnewlist=0;
        for (indexoldlist=0;indexoldlist<coldlist;indexoldlist++) {
            int y=oldList[indexoldlist]/MAX_X, x=oldList[indexoldlist]%MAX_X;
            //up
            if (y>0) {
                if (Map[y-1][x].status!=0 && Map[y-1][x].T==-1) {
                    newList[indexnewlist++]=MAX_X*(y-1)+x;
                    Map[y-1][x].T=Map[y][x].T+1;
                }
            }
            //right
            if (x<MAX_X-1) {
                if (Map[y][x+1].status!=0 && Map[y][x+1].T==-1) {
                    newList[indexnewlist++]=MAX_X*y+x+1;
                    Map[y][x+1].T=Map[y][x].T+1;
                }
            }
            //down
            if (y<MAX_Y-1) {
                if (Map[y+1][x].status!=0 && Map[y+1][x].T==-1) {
                    newList[indexnewlist++]=MAX_X*(y+1)+x;
                    Map[y+1][x].T=Map[y][x].T+1;
                }
            }
            //left
            if (x>0) {
                if (Map[y][x-1].status!=0 && Map[y][x-1].T==-1) {
                    newList[indexnewlist++]=MAX_X*y+x-1;
                    Map[y][x-1].T=Map[y][x].T+1;
                }
            }
        }
        coldlist=0;
        for (i=0;i<indexnewlist;i++) {
            oldList[coldlist++]=newList[i];
        }
        T++;
    }
    free(newList);
    free(oldList);
    char flagin=1;
    for (i=0;i<MAX_Y;i++) {
        for (j=0;j<MAX_X;j++) {
            if (Map[i][j].T==-1 && Map[i][j].status!=0) {
                flagin=0;
                break;
            }
        }
        if (!flagin) break;
    }
    for (i=0;i<MAX_Y;i++) {
        free(Map[i]);
    }
    free(Map);
    return flagin;
}

int set_count( int *count, int numberblocks, int level, int numberwalls, int *sum) {
    if (level==numberblocks) {
        if (*sum==numberwalls) return 1;
        else return 0;
    } else {
        while (1) {
            int x=5+rand()%11;
            count[level]=x;
            *sum=*sum+x;
            int q=numberwalls-*sum,w=numberblocks-(level+1);
            if (q>=5*w && q<=15*w) {
                int t=set_count(count,numberblocks,level+1,numberwalls,sum);
                if (t==1) return 1;
                else *sum=*sum-x;
            }
        }
        return 1;
    }
}





int main(int argc, char **argv) {
	const int MAX_Y=53;
	const int MAX_X=216;
	const int cplayers=2;
	int cmonsters=MAX_Y*MAX_X/100,cgolds=MAX_Y*MAX_X/100;

	srand(time(NULL));

    int i,j,indexplayer,indexmonster,indexgold;
    const char str[]="input1.txt";

    char **map;
    map=(char **)malloc(MAX_Y*sizeof(char *));
    for (i=0;i<MAX_Y;i++) {
		map[i]=(char *)malloc(MAX_X*sizeof(char));
	}
	for (i=0;i<MAX_Y;i++) {
		for (j=0;j<MAX_X;j++) {
			map[i][j]='.';
		}
	}

	//Gen_Labyrinth
    for (i=0;i<MAX_Y;i++) {
		map[i][0]='#';
		map[i][MAX_X-1]='#';
	}
	for (j=0;j<MAX_X;j++) {
		map[0][j]='#';
		map[MAX_Y-1][j]='#';
	}
	int numberwalls=MAX_Y*MAX_X/5;
    while (numberwalls>0) {
        printf("%d\n",numberwalls);
        int X=rand()%MAX_X;
        int Y=rand()%MAX_Y;
        if (map[Y][X]=='.') {
            map[Y][X]='#';
            int t=is_correct(map,MAX_Y,MAX_X);
            if (t==0) {
                map[Y][X]='.';
                numberwalls++;
            }
        } else numberwalls++;
        numberwalls--;
    }


	//Gen players
	for (indexplayer=0;indexplayer<cplayers;indexplayer++) {
	    while (1) {
	        int cur_y=rand() % MAX_Y, cur_x=rand() % MAX_X;
	        if (map[cur_y][cur_x]=='.') {
	            map[cur_y][cur_x]='0'+indexplayer;
                break;
	        }
	    }
	}
	//Gen golds
	for (indexgold=0;indexgold<cgolds;indexgold++) {
	    while (1) {
	        int cur_y=rand() % MAX_Y, cur_x=rand() % MAX_X;
	        if (map[cur_y][cur_x]=='.') {
	            map[cur_y][cur_x]='*';
                break;
	        }
	    }
	}
	//Gen monsters
	for (indexmonster=0;indexmonster<cmonsters;indexmonster++) {
	    while (1) {
	        int cur_y=rand() % MAX_Y, cur_x=rand() % MAX_X;
	        if (map[cur_y][cur_x]=='.') {
	            map[cur_y][cur_x]='@';
                break;
	        }
	    }
	}

	freopen(str,"wt",stdout);
	printf("%d %d\n",MAX_Y,MAX_X);
	for (i=0;i<MAX_Y;i++) {
		for (j=0;j<MAX_X;j++) {
			printf("%c",map[i][j]);
		}
		printf("\n");
	}
	fclose(stdout);

	return 0;
}
