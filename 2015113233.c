//2015113233  ������
/* R
//	valid == 0 -> mm���� ĳ�÷� ������ �о��
//  valid == 1
tag ==	-> ĳ�÷ΰ��� ĳ���� ���� �о��
tag !=	-> ĳ���� ���� mm�� �Űܳ��� input addr�� mm �����͸� ĳ���� �ű�
//	W
valid ==0	-> 1 word �϶��� mm���پ��� �׳� ĳ���� ������ ����
2 word �̻��϶��� mm���� ���常ŭ ������ �����ͼ� ĳ�÷� �ű��, ĳ���� write�ϱ�
valid == 1
tag ==	-> ĳ�ÿ� write�ϱ�
tag !=	-> ĳ���� ���� mm�� �Űܳ��� imput addr�� mm�����͸� ĳ���� �ű�, �׸��� ĳ���� write�Ѵ�.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*#include <sys/timeb.h>

struct timeval {
long tv_sec; // ��
long tv_usec; // ����ũ����
};*/

//2015113233  ������

typedef struct ccache {
	int valid;
	int tag;
	int* data;
}Cache;

int log2(int x) {
	int cnt = 0;
	while (1) {
		if (x == 1) {
			return cnt;
		}
		x = x / 2;
		cnt++;
	}
};
void main(int ac, char * av[]) {
	FILE * fp = fopen(av[1], "r");
	int CR, MAL, CS, CLS, SA = 0;
	char str1[100], str2[100], str3[100], str4[100], str5[100];
	fscanf(fp, "%s %d", &str1, &CR);
	fscanf(fp, "%s %d", &str2, &MAL);
	fscanf(fp, "%s %d", &str3, &CS);
	fscanf(fp, "%s %d", &str4, &CLS);
	fscanf(fp, "%s %d", &str5, &SA);
	printf("%s%d\n%s%d\n%s%d\n%s%d\n%s%d\n",
		str1, CR, str2, MAL, str3, CS, str4, CLS, str5, SA);
	float ans = 0;
	ans = MAL / CR;

	printf("Average memory access latency without cache: %.2f\n\n", ans);

	int offsetSize = 0, indexSize = 0, tagSize = 0;
	int cacheIndex = 0;
	int tagMask = 0, indexMask = 1, offsetMask = 1;
	int wordNum = 0;
	int i = 0, j = 0, k = 0;
	int TAG = 0, INDEX = 0, OFFSET = 0;
	char hexa[8] = { 0, };
	int position = 0;
	int wrCnt = 0;

	clock_t start, end;
	double res;

	start = clock();

	wordNum = CLS / 4;
	cacheIndex = CS / CLS;
	indexSize = log2(cacheIndex);
	offsetSize = log2(CLS);
	tagSize = 32 - offsetSize - indexSize;


	offsetMask = (1 << offsetMask) - 1;

	tagMask = ((1 << tagSize) - 1) << (32 - tagSize);

	indexMask = ((1 << indexSize) - 1) << offsetSize;

	Cache *cache;

	cache = malloc(sizeof(Cache)*cacheIndex);

	for (i = 0; i < cacheIndex; i++) {//ĳ�� �ʱ�ȭ
		cache[i].data = (int *)malloc(sizeof(int) * wordNum);
		cache[i].valid = 0;
		cache[i].tag = 0;
		for (j = 0; j < wordNum; j++)
			cache[i].data[j] = 0;
	}
	int *MM;
	MM = (int*)malloc(sizeof(int) * 1024 * 1024 * 16);
	for (i = 0; i < 1024 * 1024 * 16; i++) {//MM �ʱ�ȭ
		MM[i] = 0;
	}

	int accessCnt = 0;
	int missCnt = 0;
	char type;
	int addr = 0;
	int data = 0;
	int newAddr = 0;
	int orgTag = 0, orgIndex = 0, orgOffset = 0, orgAddr = 0;
	int cclock = 0;
	//2015113233  ������



	FILE* fp2 = fopen(av[2], "r");

	while (!feof(fp2)) {
		fscanf(fp2, "%c %d", &type, &addr);

		///////////////read////////////////////////////////////

		if (type == 'R') {
			accessCnt++; //������ ���� �� ++

			TAG = (tagMask & addr) >> (indexSize + offsetSize);
			INDEX = (indexMask & addr) >> offsetSize;
			OFFSET = addr & offsetMask;


			if (cache[INDEX].valid == 0) {
				missCnt += 1;//////////////////////misscnt

				cclock += SA * 2;//cache access

				cache[INDEX].valid = 1;
				cache[INDEX].tag = TAG;

				for (i = 0; i < wordNum; i++) {
					cache[INDEX].data[i] = MM[(addr / 4) + i];//���ο��� �����Ͱ�������
				}
				cclock += MAL;//mm access
			}

			else if (cache[INDEX].valid == 1) {

				if (cache[INDEX].tag != TAG) {//tag �ٸ���
					missCnt += 1;//////////////////////misscnt
					cclock += SA * 2; // cache access

					orgAddr = (((cache[INDEX].tag) << indexSize) + INDEX) << offsetSize;

					cclock += MAL;//mm access

					for (i = 0; i < wordNum; i++) {
						MM[(orgAddr / 4) + i] = cache[INDEX].data[i];//cache ���� mm���� ������ �ű��
					}
					cclock += MAL;//mm access

					for (i = 0; i < wordNum; i++) {//MM�� INDEX�� �ִ� ������ cache ������
						cache[INDEX].data[i] = MM[(addr / 4) + i];
					}

					cache[INDEX].tag = TAG;
				}

				else if (cache[INDEX].tag == TAG) {//�±� ������
					cclock += SA * 2; // cache access
				}
			}
		}

		else if (type == 'W') {
			accessCnt++; //������ ���� �� ++
			fscanf(fp2, "%d", &data);

			TAG = (tagMask & addr) >> (indexSize + offsetSize);
			INDEX = (indexMask & addr) >> offsetSize;
			//OFFSET = (addr%CLS) / 4;// offsetMask & addr;
			OFFSET = addr & offsetMask;

			newAddr = (addr&tagMask) + (addr&indexMask) + (addr&offsetMask);


			if (cache[INDEX].valid == 0) {
				missCnt += 1;//////////////////////misscnt
				cclock += SA * 2; // cache access

				cache[INDEX].tag = TAG;
				cache[INDEX].valid = 1;

				if (wordNum != 1) {//wordnum�� 2�̻��̸� mm���� ĳ�� �� ��������
					for (i = 0; i < wordNum; i++) {//MM�� INDEX�� �ִ� ������ cache ������
						cache[INDEX].data[i] = MM[(addr / 4) + i];
					}
					cclock += MAL;//mm access
				}

				cache[INDEX].data[OFFSET] = data; //cache �� ������ �ֱ�
			}

			else if (cache[INDEX].valid == 1) {

				if (cache[INDEX].tag == TAG) {//same tag
					cclock += SA * 2;//cache access
					cache[INDEX].data[OFFSET] = data; //data update
				}

				else if (cache[INDEX].tag != TAG) {//diffent tag
					missCnt += 1;//////////////////////misscnt

					cclock += SA * 2;//cache access

					orgAddr = (((cache[INDEX].tag) << indexSize) + INDEX) << offsetSize;

					for (i = 0; i < wordNum; i++) {
						MM[(orgAddr / 4) + i] = cache[INDEX].data[i];//cache ���� mm���� ������ �ű��
					}
					cclock += MAL;//mm access

					if (wordNum != 1) {
						//cclock += SA * 2;//cache access

						for (i = 0; i < wordNum; i++) {//MM�� INDEX�� �ִ� ������ cache ������
							cache[INDEX].data[i] = MM[(newAddr / 4) + i];
						}
						cclock += MAL;//mm access
					}

					cache[INDEX].data[OFFSET] = data;
					cache[INDEX].tag = TAG;
				}
			}
		}
	}
	fclose(fp2);

	printf("*L1 Cache Contents\n\n");
	for (i = 0; i < cacheIndex; i++) {
		printf("%d : ", i);
		for (j = 0; j < wordNum; j++) {
			int Pdata = cache[i].data[j]; //ĳ�� ���� �����Ͱ�
			for (k = 0; k < 8; k++) {
				int mod = Pdata % 16;

				if (mod < 10) {// �������� 10���� ������
					hexa[position] = 48 + mod;
				}
				else {
					hexa[position] = 65 + (mod - 10);
				}

				Pdata = Pdata / 16; //16���� ���� �� ����
				position++;

			}

			for (k = 7; k >= 0; k--)
				printf("%c", hexa[k]);
			printf("   ");
			position = 0;
		}
		printf("\n");
	}
	//2015113233  ������
	//2015113233  ������
	//2015113233  ������

	end = clock(); // end runtime
	res = (double)(end - start) / (CLOCKS_PER_SEC);
	//2015113233  ������
	//2015113233  ������

	printf("\nTotal program run time : %f seconds\n", res);
	printf("L1 total access : %d\n", accessCnt);//check
	printf("L1 total miss count : %d\n", missCnt);
	printf("L1 miss rate : %.2f%%\n", ((float)(missCnt) / accessCnt) * 100);
	printf("Average memory access latency : %f ns\n", cclock* (1.0 / CR) / accessCnt);
	//cclock* (1.0/CR) / accessCnt
}