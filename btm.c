#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEED 3
#define PLAYER 20
#define MATCH (PLAYER * PLAYER * 10)
#define ITERATE 10000

static double player[PLAYER];

static int win[PLAYER];
static int match[PLAYER][PLAYER];

static int
compar(const void *a, const void *b) {
	const double *aa = a;
	const double *bb = b;
	if (*aa < *bb)
		return -1;
	else if (*aa > *bb)
		return 1;
	return 0;
}

static void
random_match() {
	memset(match, 0, sizeof(match));
	int t;
	for (t=0;t<MATCH;t++) {
		int i,j;
		do {
			i = rand() % PLAYER;
			j = rand() % PLAYER;
		} while(i==j);
		double p = player[i] / (player[i] + player[j]);
		double r = (double)rand() / RAND_MAX;
		if (r < p) {
			match[i][j] ++;
		} else {
			match[j][i] ++;
		}
	}
	memset(win,0,sizeof(win));
	int i,j;
	for (i=0;i<PLAYER;i++) {
		int s = 0;
		for (j=0;j<PLAYER;j++) {
			win[i] += match[i][j];
			s += match[i][j] + match[j][i];
		}
		printf("%d: %d/%d ", i, win[i],s);
		for (j=0;j<PLAYER;j++) {
			if (match[i][j] > 0) {
				printf("(%d:%d/%d) ", j, match[i][j], match[i][j] + match[j][i]);
			}
		}
		printf("\n");
	}
}

static void
iterate(double from[PLAYER] , double to[PLAYER]) {
	int i,j;
	for (i=0;i<PLAYER;i++) {
		double sum = 0;
		for (j=0;j<PLAYER;j++) {
			double p = from[i]+from[j];
			if (p != 0) {
				sum += (match[i][j] + match[j][i])/p;
			}
		}
		if (sum == 0) {
			to[i] = 0;
		} else {
			to[i] = (double)win[i]/sum;
		}
	}
}

static void
init() {
	srand(SEED);
	int i;
	for (i=0;i<PLAYER;i++) {
		player[i] = (double)rand()/RAND_MAX * 1000;
	}
	qsort(player, PLAYER, sizeof(player[0]), compar);
	for (i=0;i<PLAYER;i++) {
		printf("%lf ", player[i]);
	}
	printf("\n");
	random_match();
}


int
main() {
	init();
	double p[PLAYER], tmp[PLAYER];
	int i;
	for (i=0;i<PLAYER;i++) {
		p[i] = 1.0 / PLAYER;
	}
	double *from = p;
	double *to = tmp;
	for (i=0;i<ITERATE;i++) {
		iterate(from, to);
		double *t = from;
		from = to;
		to = t;
	}
	for (i=0;i<PLAYER;i++) {
		printf("%lf ", from[i] * 1000);
	}
	printf("\n");
	return 0;
}
