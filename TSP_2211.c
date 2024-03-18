#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QT 10000 	//So luong ca the toi da trong quan the
#define MAX 999			//Khong co duong di tu TP nay den TP kia
#define sotp 6

//int tp[sotp][sotp] = 	{{MAX, 2, MAX, 12, 5},
//		                {2, MAX, 4, 8, MAX},
//		                {MAX, 4, MAX, 3, 3},
//		                {12, 8, 3, MAX, 10},
//		                {5, MAX, 3, 10, MAX}};

int tp[sotp][sotp] = 	{{MAX, 4, MAX, 5, MAX, 2},
						{4, MAX, 8, MAX, MAX, 3},
						{MAX, 8, MAX, MAX, 7, MAX},
						{5, MAX, MAX, MAX, MAX, 6},
						{MAX, MAX, 7, MAX, MAX, 1},
						{2, 3, MAX, 6, 1, MAX}};					
					
typedef struct{
    int A[sotp];
    int he_so_thich_nghi;
    int n; //So thanh pho
}CaThe; 

int ngau_nhien(int from, int to){
   return (rand() % (to - from + 1)) + from;
}

//Ham tra ve tong cac lan lap cua tat ca phan tu trong mang
int lap_phan_tu(int a[]){
    int i, b[sotp];
    for (i=0; i<sotp; i++)
        b[i] = 0;
    for (i=0; i<sotp; i++)
        b[a[i]-1]++;
    int sum = 0;
    for (i=0; i<sotp; i++)
        if (b[i] != 0)
            sum += b[i] -1;
    return sum;
}

int tinh_he_so_thich_nghi(int a[]){
    int sum = 0;
    int i;
    for (i=0; i<sotp-1; i++){
        int u,v;
        u = a[i]-1;
        v = a[i+1]-1;
        sum += tp[u][v];
    }
    int cnt = lap_phan_tu(a);
    sum += cnt * MAX;
    return sum;
}

void khoi_tao_ca_the(CaThe *c){
	int i;
    for(i=0; i<sotp; i++)
        c->A[i] = ngau_nhien(0,sotp-1) + 1; //Khoi tao thanh pho ngau nhien tu 1 den sotp tuong ung ABCDE...
    c->n = sotp;
    c->he_so_thich_nghi = tinh_he_so_thich_nghi(c->A);
}

void hien_thi_ca_the(CaThe c){
	int i;
    for (i=0; i<sotp; i++)
        printf("%c ",c.A[i] + 64); //Chuyen cac so 12345... thanh ABCDE...
    printf("\t%d\n",c.he_so_thich_nghi);
}

typedef struct{
    CaThe CT[MAX_QT];
    int size;
}QuanThe;

void init_QT(QuanThe *qt, int n){
    qt->size = n;
}

void khoi_tao_quan_the(QuanThe *qt, int n){
	int i;
    for (i=0; i<n; i++)
        khoi_tao_ca_the(&qt->CT[i]);
    init_QT(qt,n);
}

//Sap xep quan the dua tren He so thich nghi
void sap_xep_quan_the(QuanThe *qt){
	int i, j;
    for (i=0; i<qt->size-1; i++)
        for (j=i+1; j<qt->size; j++)
            if (qt->CT[i].he_so_thich_nghi > qt->CT[j].he_so_thich_nghi){
                CaThe temp = qt->CT[i];
                qt->CT[i] = qt->CT[j];
                qt->CT[j] = temp;
            }
}

//Khai bao cau truc danh sach luu 2 Ca The da lai ghep
typedef struct{
    CaThe E[2];
    int size;
} List;

void init_List(List *L){
    L->size = 0;
}

void append_List(List *L, CaThe x){
    L->E[L->size++] = x;
}

List lai(CaThe c1, CaThe c2){
	int i;
    CaThe cr1 = c1;
    CaThe cr2 = c2;
    List l;
    init_List(&l);
    int index = ngau_nhien(0,sotp-1);
    int temp[index+1];
    //Lai giua ca the cr1 va ca the cr2
    for (i=0; i<=index; i++)
        temp[i] = cr1.A[i];
    for (i=0; i<=index; i++)
        cr1.A[i] = cr2.A[i];
    for (i=0; i<=index; i++)
        cr2.A[i] = temp[i];
    cr1.he_so_thich_nghi = tinh_he_so_thich_nghi(cr1.A);
    cr2.he_so_thich_nghi = tinh_he_so_thich_nghi(cr2.A);
    //Them 2 ca the moi vao ds
    append_List(&l,cr1);
    append_List(&l,cr2);
    return l;
}

//Thay doi 1 gen ngau nhien trong CaThe voi so lan ngau nhien
void dot_bien(CaThe *c){
    int index = ngau_nhien(0,sotp-1);
    int count = ngau_nhien(1, 10);
    int value;
    while(count > 0){
        value = ngau_nhien(1,sotp);
        c->A[index] = value;
        count--;
    }
    c->he_so_thich_nghi = tinh_he_so_thich_nghi(c->A);
}

CaThe di_truyen(int slct, int TheHe){
	int i, j;
    QuanThe qt;
    CaThe tot;
    khoi_tao_quan_the(&qt,slct);
    sap_xep_quan_the(&qt);
    tot = qt.CT[0]; //Mac dinh ca the dau tien la tot nhat
    for (i=1; i<=TheHe; i++){ 
        sap_xep_quan_the(&qt);
        if (qt.CT[0].he_so_thich_nghi < tot.he_so_thich_nghi)
            tot = qt.CT[0];
        QuanThe new;
        init_QT(&new,slct);
        for (j=0; j<slct; j+=2){
            int cha, me;
            while (1){
                cha = ngau_nhien(0, slct-1);
                me = ngau_nhien(0, slct-1);
                if (cha != me)
                    break;
            }
            List l = lai(qt.CT[cha],qt.CT[me]); //List l se chua 2 ca the lai ghep tu cha va me
            dot_bien(&l.E[0]);
            dot_bien(&l.E[1]);
            new.CT[j] = l.E[0];
            new.CT[j+1] = l.E[1];
        }
        qt = new;	//Quan the New cung se co 100 con 
    }
    return tot;
}

void TSP(){
	int slct, TheHe;
	QuanThe q;
    int i;
    printf("TSP Genertic Alrogithm\n");
    printf("\n--------------------------------\n");
    while(1){
    	printf("Nhap so luong ca the trong quan the: ");
    	scanf("%d", &slct);
    	if(slct >1)
    		break;
    	printf("Nhap so ca the lon hon 1 moi co the lai ghep!!\n");
	}
    printf("Nhap so the he muon lai den: ");
    scanf("%d", &TheHe);
    printf("--------------------------------\n");
    printf("\nQuan the ban dau:\n\n");
    printf("STT\tDuong di\tDo thich nghi\n");
    khoi_tao_quan_the(&q,slct);
   	sap_xep_quan_the(&q);
   	for (i=0; i<q.size; i++){
   		printf("%d\t", i+1);
    	hien_thi_ca_the(q.CT[i]);
	}
    CaThe kq = di_truyen(slct,TheHe);
    printf("\nLo trinh duong di ngan nhat: \n");
    printf("\t");
    hien_thi_ca_the(kq);
}

int main(){
	//Khoi tao gia tri ngau nhien khac nhau sau moi lay chay chtr
	srand((unsigned int)time(NULL));
	TSP();
	return 0;
}