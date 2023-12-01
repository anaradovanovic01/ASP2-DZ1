#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct TreeNode {
	int info;
	int b;
	int flag;
	struct TreeNode *left, *right;
} TreeNode;

typedef struct StackNode {
	struct TreeNode* info;
	struct StackNode *next;
} StackNode;

//F-JE ZA STEK
void PushN(struct StackNode** stack, struct TreeNode* x) {
	struct StackNode* p;
	p = (struct StackNode*)malloc(sizeof(struct StackNode));
	if (p == NULL) {
		printf("Neuspesna alokacija\n");
		exit(1);
	}
	p->info = x;
	p->next = *stack;
	*stack = p;
	return;
}

struct TreeNode* PopN(struct StackNode** stack) {
	struct StackNode* p;
	struct TreeNode* x;
	if (*stack == NULL) {
		printf("Underflow\n");
		exit(1);
	}
	else {
		p = *stack;
		*stack = p->next;
		x = p->info;
		free(p);
		return x;
	}
}

int StackEmptyN(struct StackNode** stack) {
	if (*stack == NULL) return 1;
	else return 0;
}

//F_JE ZA RED
void Insert(struct StackNode** front, struct StackNode** rear, struct TreeNode* x) {
	struct StackNode* p;
	p = (struct StackNode*)malloc(sizeof(struct StackNode));
	if (p == NULL) {
		printf("Neuspesna alokacija\n");
		exit(1);
	}
	p->info = x;
	p->next = NULL;
	if (*rear == NULL)
		*front = p;
	else
		(*rear)->next = p;
	*rear = p;
	return;
}

struct TreeNode* Delete(struct StackNode** front, struct StackNode** rear) {
	struct StackNode* p;
	struct TreeNode* x;
	if (*front == NULL) {
		printf("Underflow\n");
		exit(1);
	}
	else {
		p = *front;
		x = p->info;
		*front = p->next;
		if (*front == NULL)
			*rear = NULL;
		free(p);
		return x;
	}
}

//F_JE ZA STABLO
TreeNode* GetTreeNode() {
	TreeNode* p;
	p = (TreeNode*)malloc(sizeof(TreeNode));
	if (p == NULL) {
		printf("Neuspesna alokacija\n");
		exit(1);
	}
	p->left = NULL;
	p->right = NULL;
	p->b = 0;
	p->flag = 0;
	return p;
}

void RightRotation(TreeNode* x) {
	TreeNode *y, *t;
	y = x->left;
	t = y->right;
	y->right = x;
	x->left = t;
}

void LeftRotation(TreeNode* x) {
	TreeNode *y, *t;
	y = x->right;
	t = y->left;
	y->left = x;
	x->right = t;
}

TreeNode* InsertNode(TreeNode* root, int k) {
	TreeNode *fp, *fx, *p, *x, *q, *y;
	int imbal;
	fp = fx = NULL;
	p = root;
	x = p; 
	while (p != NULL) {
		if (k <= p->info) q = p->left;
		else q = p->right;
		if (q != NULL) {
			if (q->b != 0) {
				fx = p;
				x = q;
			}
		}
		fp = p;
		p = q;
	}
	q = GetTreeNode();
	q->info = k;
	if (k <= fp->info) fp->left = q;
	else fp->right = q;
	if (k <= x->info) p = x->left;
	else p = x->right;
	y = p;
	while (p != q) {
		if (k <= p->info) { p->b = 1; p = p->left; }
		else { p->b = -1; p = p->right; }
	}
	if (k <= x->info) imbal = 1;
	else imbal = -1;
	if (x->b == 0) {
		x->b = imbal;
		return root;
	}
	if (x->b != imbal) {
		x->b = 0;
		return root;
	}
	if (y->b == imbal) {
		p = y;
		if (imbal == 1) RightRotation(x);
		else LeftRotation(x);
		x->b = y->b = 0;
	}
	else {
		if (imbal == 1) {
			p = y->right;
			LeftRotation(y);
			x->left = p;
			RightRotation(x);
		}
		else {
			p = y->left;
			x->right = p;
			RightRotation(y);
			LeftRotation(x);
		}
		if (p->b == 0) x->b = y->b = 0;
		else {
			if (p->b == imbal) {x->b = -imbal; y->b = 0;}
			else { x->b = 0; y->b = imbal; }
			p->b = 0;
		}
	}
	if (fx == NULL) root = p;
	else {
		if (x == fx->right) fx->right = p;
		else fx->left = p;
	}
	return root;
}

TreeNode* InitTree(int** mat, int m, int n) {
	TreeNode* koren = GetTreeNode();
	koren->info = mat[0][0];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (i == 0 && j == 0) continue;
			koren = InsertNode(koren, mat[i][j]);
		}
	}
	return koren;
}

int TreeHeight(struct TreeNode* root) {
	if (root == NULL)
		return 0;
	int i, height, queueSize;
	struct TreeNode* p = NULL;
	struct StackNode* front = NULL;
	struct StackNode* rear = NULL;
	Insert(&front, &rear, root);
	queueSize = 1;
	height = 0;
	while ((front != NULL) && (rear != NULL)) {
		for (i = queueSize; i > 0; i--) {
			p = Delete(&front, &rear);
			queueSize--;
			if (p->left != NULL) {
				Insert(&front, &rear, p->left);
				queueSize++;
			}
			if (p->right != NULL) {
				Insert(&front, &rear, p->right);
				queueSize++;
			}
		}
		height++;
	}
	return height;
}

void PrintSpaces(int n) {
	for (int i = 0; i < n; i++)
		printf("  ");
	return;
}

void PrintTree(struct TreeNode* root) {
	if (root == NULL) {
		printf("0\n");
		return;
	}
	int height, level, nextLevelFlag;
	struct TreeNode *p, *q;
	struct StackNode* front = NULL;
	struct StackNode* rear = NULL;
	height = TreeHeight(root);
	Insert(&front, &rear, root);
	Insert(&front, &rear, NULL);
	level = 0; nextLevelFlag = 0;
	while (level < height) {
		p = Delete(&front, &rear);
		if (p == NULL) {
			level++;
			nextLevelFlag = 0;
			printf("\n");
			if ((front != NULL) && (rear != NULL))
				Insert(&front, &rear, NULL);
			continue;
		}
		else {
			if (nextLevelFlag == 0)
				PrintSpaces(pow(2, (height - level - 1)) - 1);
			else
				PrintSpaces(pow(2, (height - level)) - 1);
			nextLevelFlag = 1;
			if (p->info >= 0)  printf("%2d", p->info);
			else printf("  ");
		}
		if (p->left != NULL)
			Insert(&front, &rear, p->left);
		else {
			q = GetTreeNode();
			q->info = -1;
			Insert(&front, &rear, q);
		}
		if (p->right != NULL)
			Insert(&front, &rear, p->right);
		else {
			q = GetTreeNode();
			q->info = -1;
			Insert(&front, &rear, q);
		}
	}
	while ((front == NULL) && (rear == NULL))
		Delete(&front, &rear);
	return;
}

void DeallocateTree(struct TreeNode* root) {
	if (root == NULL) return;
	int i = 0;
	struct StackNode* stack = NULL;
	struct TreeNode *p = root;
	while (p != NULL) {
		PushN(&stack, p);
		p = p->left;
	}
	while (!(StackEmptyN(&stack))) {
		p = PopN(&stack);
		if (p->flag == 0) {
			p->flag = 1;
			PushN(&stack, p);
			p = p->right;
			while (p != NULL) {
				PushN(&stack, p);
				p = p->left;
			}
		}
		else {
			p->flag = 0;
			free(p);
		}
	}
	return;
}

TreeNode* FindKeyTree(TreeNode* root, int k, int* stepcounter) {
	TreeNode* p;
	p = root;
	*stepcounter = 0;
	while (p != NULL && k != p->info) {
		(*stepcounter)++;
		if (k < p->info) p = p->left;
		else p = p->right;
	}
	return p;
}

//F_JE ZA MATRICU
int** InputMatrixFromStd(int* m, int* n) {
	int** mat; int i, j;
	printf("Unesite dimenzije matrice: ");
	scanf("%d%d", m, n);
	if (*n <= 0 || *m <= 0) return 0;
	mat = malloc(*m * sizeof(int*));
	if (mat == NULL) {
		printf("Neuspesno alociranje protora\n");
		return 0;
	}
	for (i = 0; i < *m; i++) {
		mat[i] = malloc(*n * sizeof(int));
		if (mat[i] == NULL) {
			printf("Neuspesno alociranje protora\n");
			return 0;
		}
		for (j = 0; j < *n; j++)
			scanf("%d", &mat[i][j]);
	}
	return mat;
}

int** InputMatrixFromFile(int* m, int* n) {
	int** mat; int i, j;
	char name[30];
	FILE *in;
	printf("Unesite ime datoteke koja sadrzi matricu: ");
	scanf("%s", name);
	in = fopen(name, "r");
	if (!in) {
		perror(NULL);
		exit(1);
	}
	fscanf(in, "%d %d", m, n);
	if (*n <= 0 || *m <= 0) return 0;
	mat = malloc(*m * sizeof(int*));
	if (mat == NULL) {
		printf("Neuspesno alociranje protora\n");
		return 0;
	}
	for (i = 0; i < *m; i++) {
		mat[i] = malloc(*n * sizeof(int));
		if (mat[i] == NULL) {
			printf("Neuspesno alociranje protora\n");
			return 0;
		}
		for (j = 0; j < *n; j++)
			fscanf(in, "%d ", &mat[i][j]);
	}
	fclose(in);
	return mat;
}

int** GenerateMatrix(int* m, int* n) { 
	int** mat; int i, j, l;
	printf("Unesite dimenzije matrice: ");
	scanf("%d%d", m, n);
	if (n <= 0 || m <= 0) return 0;
	mat = malloc(*m * sizeof(int*));
	if (mat == NULL) {
		printf("Neuspesno alociranje protora\n");
		return 0;
	}
	for (i = 0; i < *m; i++) {
		mat[i] = malloc(*n * sizeof(int));
		if (mat[i] == NULL) {
			printf("Neuspesno alociranje protora\n");
			return 0;
		}
	}
	mat[*m - 1][*n - 1] = (rand() % 20) + 1;
	for (i = 0; i < *m; i++) {
		for (j = 0; j < *n; j++) {
			if (i == 0 && j == 0) continue;
			if (i == 0) l = mat[*m-i-1][*n-j];
			else if (j == 0) l = mat[*m-i][*n-j-1];
			else l = mat[*m-i-1][*n-j] > mat[*m-i][*n-j-1] ? mat[*m-i-1][*n-j] : mat[*m-i][*n-j-1];
			mat[*m-i-1][*n-j-1] = (rand() % ((i + j + 2) * 10 - l - 1)) + l;
		}
	}
	return mat;
}

int FindKeyMatrix(int** mat, int m, int  n, int *i, int  *j, int key, int* stepcounter) {
	int curr;
	*stepcounter = 0;
	*i = 0;
	*j = n - 1;
	while (*i < m && *j < n && *i >= 0 && *j >= 0) {
		curr = mat[*i][*j];
		(*stepcounter)++;
		if (curr == key) return 1;
		else if (curr > key) (*i)++;
		else (*j)--;
	}
	return 0;
}

double EvalMatrix(int** mat, int m, int n) {
	int i, j, stepcounter, key, lower, upper, num, s = 0; double average;
	printf("Unesite broj kljuceva koje treba izgenerisati za proveru preformansi: ");
	scanf("%d", &num);
	printf("Unesite donju i gornju granicu opsega iz kog se generisu kljucevi: ");
	scanf("%d%d", &lower, &upper);
	printf("Generisani kljucevi su: ");
	for (int k = 0; k < num; k++) {
		key = (rand() % (upper - lower + 1)) + lower;
		printf("%d ", key);
		FindKeyMatrix(mat, m, n, &i, &j, key, &stepcounter);
		s += stepcounter;
	}
	average = (double)s / num;
	return average;
}

void ComparePerformance(int** mat, int m, int n, TreeNode* root) {
	int u, l, num, keys[20] = { 0 }, sm = 0, st = 0, i, j, scm, sct;
	double am, at;
	char c = ' ';
	printf("Unesite 0 za unos kljuceva sa standardnog ulaza ili 1 za unos iz datoteke: ");
	scanf("%d", &u);
	if (u) {
		char name[30];
		FILE *in;
		printf("Unesite ime datoteke koja sadrzi niz kljuceva: ");
		scanf("%s", name);
		in = fopen(name, "r");
		if (!in) {
			perror(NULL);
			exit(1);
		}
		l = 0;
		while (c != EOF) {
			fscanf(in, "%d", &keys[l]);
			l++;
			c = fgetc(in);
		}
		fclose(in);
	}
	else {
		printf("Unesite broj kljuceva koje unosite: ");
		scanf("%d", &num);
		printf("Unesite kljuceve: ");
		for (l = 0; l < num; l++) scanf("%d", &keys[l]);
	}
	for (int k = 0; k < l; k++){
		FindKeyMatrix(mat, m, n, &i, &j, keys[k], &scm);
		FindKeyTree(root, keys[i], &sct);
		sm += scm;
		st += sct;
	}
	am = (double)sm / l;
	at = (double)st / l;
	printf("\nProsecan broj koraka za matricu je %.2lf, a za stablo je %.2lf\n", am, at);
	return;
}

int main() {
	int flag = 1, u, i, j, m, n, key, stepcounter;
	double average;
	int** mat = NULL;
	TreeNode *root = NULL;
	printf("Meni:\n1. Unos matrice sa standardnog ulaza\n2. Unos matrice iz datoteke\n3. Generisanje matrice\n4. Pretraga matrice na zadati kljuc\n"
		"5. Evaluacija preformansi pretrazivanja u matrici koriscenjem nasumicno generisanih kljuceva iz nekog opsega\n6. Formiranje i ispis stabla\n"
		"7. Pretraga stabla na zadati kljuc\n8. Umetanje novog cvora u stablo\n9. Uporedjivanje preformansi stabla i matrice\n0. Kraj programa\n");
	while (flag) {
		printf("\nOdaberite stavku iz menija: ");
		scanf("%d", &u);
		switch (u) {
		case 1: { //unos matrice sa standardnog ulaza
			mat = InputMatrixFromStd(&m, &n);
			printf("Matrica je uneta:\n");
			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {
					printf("%3d", mat[i][j]);
					if (j != n - 1)
						printf(" ");
				}
				printf("\n");
			}
			break;
		}
		case 2: { //unos matrice iz datoteke
			mat = InputMatrixFromFile(&m, &n);
			printf("Matrica je uneta:\n");
			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {
					printf("%3d", mat[i][j]);
					if (j != n - 1)
						printf(" ");
				}
				printf("\n");
			}
			break;
		}
		case 3: { //generisanje matrice
			mat = GenerateMatrix(&m, &n);
			printf("Izgenerisana matrica je:\n");
			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {
					printf("%3d", mat[i][j]);
					if (j != n - 1)
						printf(" ");
				}
				printf("\n");
			}
			break;
		}
		case 4: { //pretraga matrice na zadati kljuc
			printf("Unesite kljuc na koji se vrsi pretraga: ");
			scanf("%d", &key);
			int p = FindKeyMatrix(mat, m, n, &i, &j, key, &stepcounter);
			if (p) printf("Pozicija nadjenog elementa je: %d, %d\n", i, j);
			else printf("Kljuc nije nadjen\n");
			break;
		}
		case 5: { //evaluacija preformansi pretrazivanja u matrici
			average = EvalMatrix(mat, m, n);
			printf("\nProsecan broj koraka je %.2lf\n", average);
			break;
		}
		case 6: { //formiranje i ispis stabla 
			root = InitTree(mat, m, n);
			printf("Formirano stablo: \n");
			PrintTree(root);
			break;
		}
		case 7: { //pretraga stabla na zadati kljuc
			printf("Unesite kljuc na koji se vrsi pretraga: ");
			scanf("%d", &key);
			TreeNode* q = FindKeyTree(root, key, &stepcounter);
			if (q) printf("Kljuc je nadjen\n");
			else printf("Kljuc nije nadjen\n");
			break;
		}
		case 8: { //umetanje novog cvora u stablo
			printf("Unesite kljuc novog cvora: ");
			scanf("%d", &key);
			root = InsertNode(root, key);
			printf("Cvor je dodat: \n");
			PrintTree(root);
			break;
		}
		case 9: { //uporedjivanje preformansi stabla i matrice 
			ComparePerformance(mat, m, n, root);
			break;
		}
		case 0: { //kraj
			flag = 0;
			if (mat != NULL) {
				for (i = 0; i < m; i++) free(mat[i]);
				free(mat);
			}
			DeallocateTree(root);
			break;
		}
		default: {
			printf("Uneta opcija ne postoji u meniju, pokusajte ponovo\n");
		}
		}
	}
	return 0;
}
