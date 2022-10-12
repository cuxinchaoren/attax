/*
    Author : Robert_Yuan
*/

#include<cstdio>
#include<cstring>
#include<algorithm>

using namespace std;

const int INF=0x3f3f3f3f;

int map[9][9],TTT[9][9];
int cnt[3];
int mvx[25],mvy[25];
int arx[9],ary[9];
int move_list[25],top;

void init();//��ʼ��
void menu();//��������
void sum();//ͳ��������Ŀ
void print(int map[][9]);//�������
void print2(int map[][9]);//���һ���ÿ�������
void operation1();//������Ϸ
void operation2();//˫����Ϸ
void Copy();
void Save1();//�浥����Ϸ��
void Save2();//��˫����Ϸ��
bool can_move(int x,int y);
void Search1(int Depth);
void Search2(int Depth);

int main(){
    //freopen("bug.in","r",stdin);
    //freopen("game.out","w",stdout);
//    init();
    menu();
    //operation1();
}

void menu(){
    int ord1,ord2;
    printf(" ������������������������������������������������ \n");
    printf("��ӭ����ͬ����С��Ϸ�������������������������²���\n");
    printf(" ������������������������������������������������\n");
    printf("    0.�˳���Ϸ\n");
    printf("    1.������Ϸ\n");
    printf("    2.˫����Ϸ\n");
    scanf("%d",&ord1);
    if(ord1==1){
        printf("     ����������������������������������������������\n");
        printf("    �������˵�����Ϸ�������������������������²���\n");
        printf("     ����������������������������������������������\n");
        printf("        0.�����ϲ�\n");
        printf("        1.����Ϸ\n");
        printf("        2.��ȡ�浵\n");
        scanf("%d",&ord2);
        if(ord2==0) menu();//�����ϲ㼴�ٴεݹ�
        else{
            init();
            if(ord2==2){//��ȡ�浵
                FILE *fp;
                fp=fopen("rec1.txt","r");
                for(int i=1;i<=7;i++)
                    for(int j=1;j<=7;j++)
                        fscanf(fp,"%d",&map[i][j]);
            }
            operation1();
        }
    }
    else if(ord1==2){
        printf("     ����������������������������������������������\n");
        printf("    ��������˫����Ϸ�������������������������²���\n");
        printf("     ����������������������������������������������\n");
        printf("        0.�����ϲ�\n");
        printf("        1.����Ϸ\n");
        printf("        2.��ȡ�浵\n");
        scanf("%d",&ord2);
        if(ord2==0) menu();
        else{
            init();
            if(ord2==2){
                FILE *fp;
                fp=fopen("rec2.txt","r");
                for(int i=1;i<=7;i++)
                    for(int j=1;j<=7;j++)
                        fscanf(fp,"%d",&map[i][j]);
            }
            operation2();
        }
    }
    else
        return ;
}

//��ʼ������
void init(){
    //��ʼ������
    map[1][7]=map[7][1]=1;
    map[1][1]=map[7][7]=2;
    //��ʼ���ƶ�����Χ����
    int tmp=0;
    for(int i=-2;i<=2;i++)
        for(int j=-2;j<=2;j++)
            if(!(i==0 && j==0)) tmp++,mvx[tmp]=i,mvy[tmp]=j; //mvx��mvy�ֱ��ʾ�����ƶ�ʱx�����Ϻ�y�����ϵ��ƶ�
    tmp=0;
    for(int i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++)
            if(!(i==0 && j==0)) tmp++,arx[tmp]=i,ary[tmp]=j; //arx��ary�ֱ��ʾȾɫʱ��x��y�ϵ��ƶ�
}

//ͳ�������ڸ����ӵ���Ŀ
void sum(int map[][9]){
    cnt[0]=cnt[1]=cnt[2]=0;
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++)
            cnt[map[i][j]]++;
}

//���һ����ª������(@��#���)
void print(int map[][9]){
    sum(map);
    printf("YOU:%2d  COM:%2d\n",cnt[1],cnt[2]);
    putchar(' ');
    for(int i=1;i<=7;i++)
        printf(" %d",i);
    putchar('\n');
    for(int i=1;i<=7;i++){
        printf("%d",i);
        for(int j=1;j<=7;j++){
            if(map[i][j]==1) printf(" @");
            else if(map[i][j]==2) printf(" #");
            else printf("  ");
        }
        putchar('\n');
    }
}

//���һ��Ư�������̣�ʹ���Ʊ��
void print2(int map[][9]){
    sum(map);
    printf(" �������������������������������� \n");
    printf(" YOU: %2d      ��COM: %2d\n",cnt[1],cnt[2]);
    printf(" �������������������������������� \n");
    printf("     1   2   3   4   5   6   7  \n");
    printf("   �������ש����ש����ש����ש����ש����ש�����\n");
    printf(" 1 ��");
    for(int i=1;i<=7;i++)
        if(map[1][i]==1) printf("��");
        else if(map[1][i]==2) printf("��");
        else printf("  ��");
    putchar('\n');
    for(int i=2;i<=7;i++){
        printf("   �ǩ����贈���贈���贈���贈���贈���贈����\n");
        printf(" %d ��",i);
        for(int j=1;j<=7;j++)
            if(map[i][j]==1) printf("��");
            else if(map[i][j]==2) printf("��");
            else printf("  ��");
        putchar('\n');
    }
    printf("   �������ߩ����ߩ����ߩ����ߩ����ߩ����ߩ�����\n");
}

//�ж�һ��λ�õ����Ƿ���λ�ÿ����ƶ� ��ͬʱ�������ƶ��ķ������move_list��
bool can_move(int x,int y){
    top=0;
    int nx,ny;
    for(int i=1;i<=24;i++){
        nx=x+mvx[i],ny=y+mvy[i];
        if(nx>=1 && nx<=7 && ny>=1 && ny<=7 && map[nx][ny]==0)
            move_list[++top]=i;
    }
    return top!=0;
}

//����һ����(x1,y1)�ƶ���(x2,y2)��t��������������Ч��
void Deal_With(int x1,int y1,int x2,int y2,int t,int map[][9]){
    map[x1][y1]=t*(abs(x2-x1)<=1 && abs(y2-y1)<=1);
    map[x2][y2]=t;
    int nx,ny;
    for(int i=1;i<=8;i++){
        nx=x2+arx[i],ny=y2+ary[i];
        if(map[nx][ny]) map[nx][ny]=t;
    }
}

//������Ϸ�浵
void Save1(){
    FILE *fp;
    fp=fopen("rec1.txt","w");
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            fprintf(fp,"%d ",map[i][j]);
        fprintf(fp,"\n");
    }
}

//˫����Ϸ�浵
void Save2(){
    FILE *fp;
    fp=fopen("rec2.txt","w");
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            fprintf(fp,"%d ",map[i][j]);
        fprintf(fp,"\n");
    }
}

//������������
void Copy(int A[][9],int B[][9]){
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++)
            B[i][j]=A[i][j];
}

//����0��ʹ�ú��������ս��
void COMPUTER0(){
    printf("�����ǵ���0ʱ�䣡");
    int BEST_I,BEST_J,BEST_NI,BEST_NJ,BEST=0;
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++)
            if(map[i][j]==2){
                if(can_move(i,j)){
                    for(int k=1;k<=top;k++){
                        int nx=i+mvx[move_list[k]],ny=j+mvy[move_list[k]];
                        Copy(map,TTT);
                        Deal_With(i,j,nx,ny,2,TTT);
                        sum(TTT);
                        if(cnt[2]>BEST)
                            BEST=cnt[2],BEST_I=i,BEST_J=j,BEST_NI=nx,BEST_NJ=ny;
                    }
                }
            }
    Deal_With(BEST_I,BEST_J,BEST_NI,BEST_NJ,2,map);
    printf("���԰�(%d,%d)�ƶ�����(%d,%d):\n",BEST_I,BEST_J,BEST_NI,BEST_NJ);
}

//����1��ʹ�ú����ȥ�������ս��
void COMPUTER1(){
    printf("�����ǵ���1ʱ�䣡");
    int BEST_I,BEST_J,BEST_NI,BEST_NJ,BEST=-49;
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++)
            if(map[i][j]==2){
                if(can_move(i,j)){
                    for(int k=1;k<=top;k++){
                        int nx=i+mvx[move_list[k]],ny=j+mvy[move_list[k]];
                        Copy(map,TTT);
                        Deal_With(i,j,nx,ny,2,TTT);
                        sum(TTT);
                        if(cnt[2]-cnt[1]>BEST)
                            BEST=cnt[2]-cnt[1],BEST_I=i,BEST_J=j,BEST_NI=nx,BEST_NJ=ny;
                    }
                }
            }
    Deal_With(BEST_I,BEST_J,BEST_NI,BEST_NJ,2,map);
    printf("���԰�(%d,%d)�ƶ�����(%d,%d):\n",BEST_I,BEST_J,BEST_NI,BEST_NJ);
}

int height[9],ins[9],ht;//height��ht��ʾά���ĵ���ջ��ջ��ָ�룬ins��ʾÿ��λ���ܹܵ�����ǰ���λ��
int Left[9][9];//Left��ʾͳ�����λ������������Ϊ����

//��������ֵ�е����ֵ
int Max(int x,int y){
    return x>y?x:y;
}

int Min(int x,int y){
    return x<y?x:y;
}

//����map��t��������ʹ�õ���ջ
int calcu_matrix(int map[][9],int t){
    int ans=0;
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++){
            Left[i][j]=0;
            if(map[i][j]==t)
                Left[i][j]=Left[i][j-1]+1;
        }
    for(int i=1;i<=7;i++){
        ht=0;
        for(int j=1;j<=8;j++){
            while(Left[height[ht]][i]>=Left[j][i] && ht>0){
                ans=Max(ans,Left[height[ht]][i]*(j-ins[height[ht]]));
                ht--;
            }
            ins[j]=height[ht]+1;
            ans=Max(Left[j][i]*(j-ins[j]+1),ans);
            height[++ht]=j;

        }
    }
    return ans;
}

//COM1�Ż��������Ӽ�������ͬʱ������ͨ����������еڶ����ж�
void COMPUTER2(){
    printf("�����ǵ���2ʱ�䣡");
    int BEST_I,BEST_J,BEST_NI,BEST_NJ,BEST=-49,BESTC=0;
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++)
            if(map[i][j]==2){
                if(can_move(i,j)){
                    for(int k=1;k<=top;k++){
                        int nx=i+mvx[move_list[k]],ny=j+mvy[move_list[k]],nc;
                        Copy(map,TTT);
                        Deal_With(i,j,nx,ny,2,TTT);
                        nc=calcu_matrix(TTT,2);
                        sum(TTT);
                        if(cnt[2]-cnt[1]>BEST)
                            BEST=cnt[2]-cnt[1],BEST_I=i,BEST_J=j,BEST_NI=nx,BEST_NJ=ny,BESTC=nc;
                        else if(cnt[2]-cnt[1]==BEST && BESTC<nc){
                            BEST_I=i,BEST_J=j,BEST_NI=nx,BEST_NJ=ny,BESTC=nc;
                        }
                    }
                }
            }
    Deal_With(BEST_I,BEST_J,BEST_NI,BEST_NJ,2,map);
    printf("���԰�(%d,%d)�ƶ�����(%d,%d):\n",BEST_I,BEST_J,BEST_NI,BEST_NJ);
}
int move_steps(int x,int y){
    top=0;
    int nx,ny;
    for(int i=1;i<=24;i++){
        nx=x+mvx[i],ny=y+mvy[i];
        if(nx>=1 && nx<=7 && ny>=1 && ny<=7 && !map[nx][ny]) top++;
    }
    return top;
}

int Move_power(int t){
    int cnt=0;
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++)
            if(map[i][j]==t) cnt+=move_steps(i,j);
    return cnt;
}

int Judge(int map[][9]){
    sum(map);
    int Main=cnt[2]-cnt[1],Other=calcu_matrix(map,2),Another;
    Another=0;
    //Another=Move_power(2);
    return Main*100+Other+Another;
}

int rec[10][9][9],Inform[9][2];
int Greed[25];

int Calcu_Next(int i,int j,int k,int Depth){
    int nx=i+mvx[move_list[k]],ny=j+mvy[move_list[k]];
    Deal_With(i,j,nx,ny,map[i][j],map);
    int ans=Judge(map);
    Copy(rec[Depth],map);
    return ans;
}

int MVL[10][25],TP[10];
bool can_move2(int x,int y,int Depth){
    TP[Depth]=0;
    int nx,ny;
    for(int i=1;i<=24;i++){
        nx=x+mvx[i],ny=y+mvy[i];
        if(nx>=1 && nx<=7 && ny>=1 && ny<=7 && map[nx][ny]==0)
            MVL[Depth][++TP[Depth]]=i;
    }
    return TP[Depth]!=0;
}

bool cmp1(const int &A,const int &B){
    return Greed[A]>Greed[B];
}
bool cmp2(const int &A,const int &B){
    return Greed[A]<Greed[B];
}

//��������ǵ÷���
void Search1(int Depth){
    if(Depth>3){
        Inform[Depth-1][1]=Min(Inform[Depth-1][1],Judge(map));
        return ;
    }
    Copy(map,rec[Depth]);
    //�������(����)��
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            if(map[i][j]==2 && can_move2(i,j,Depth)){
                for(int k=1;k<=TP[Depth];k++)
                    Greed[k]=Calcu_Next(i,j,k,Depth);
                //��̰�������׵÷ֵ�����
                for(int k=1;k<=TP[Depth];k++)
                    for(int l=1;l<=TP[Depth]-k;l++)
                        if(Greed[l]<Greed[l+1]){
                            int tmp=Greed[l];Greed[l]=Greed[l+1];Greed[l+1]=tmp;
                            tmp=MVL[Depth][l];MVL[Depth][l]=MVL[Depth][l+1];MVL[Depth][l+1]=tmp;
                        }

                for(int k=1;k<=TP[Depth];k++){
                    int nx=i+mvx[MVL[Depth][k]],ny=j+mvy[MVL[Depth][k]],Interest;
                    Deal_With(i,j,nx,ny,2,map);
                    Inform[Depth+1][0]=-INF;
                    Inform[Depth+1][1]=INF;

                    Search2(Depth+1);
                    Copy(rec[Depth],map);
                    if(Inform[Depth-1][1]<=Inform[Depth][0]) return;

                }
            }
    }
    Inform[Depth-1][1]=Min(Inform[Depth-1][1],Inform[Depth][0]);
}

void Search2(int Depth){
    if(Depth>3){
        Inform[Depth-1][0]=Max(Inform[Depth-1][0],Judge(map));
        return ;
    }
    Copy(map,rec[Depth]);
    //������谭����
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            if(map[i][j]==1 && can_move2(i,j,Depth)){
                for(int k=1;k<=TP[Depth];k++)
                    Greed[k]=Calcu_Next(i,j,k,Depth);
                //��̰�������Ե÷ֵ�����
                for(int k=1;k<=TP[Depth];k++)
                    for(int l=1;l<=TP[Depth]-k;l++)
                        if(Greed[l]>Greed[l+1]){
                            int tmp=Greed[l];Greed[l]=Greed[l+1];Greed[l+1]=tmp;
                            tmp=MVL[Depth][l];MVL[Depth][l]=MVL[Depth][l+1];MVL[Depth][l+1]=tmp;
                        }

                for(int k=1;k<=TP[Depth];k++){
                    int nx=i+mvx[MVL[Depth][k]],ny=j+mvy[MVL[Depth][k]],Interest;
                    Deal_With(i,j,nx,ny,1,map);
                    Inform[Depth+1][0]=-INF;
                    Inform[Depth+1][1]=INF;

                    Search1(Depth+1);
                    Copy(rec[Depth],map);
                    if(Inform[Depth-1][0]>=Inform[Depth][1]) return;
                }
            }
    }
    Inform[Depth-1][0]=Max(Inform[Depth-1][0],Inform[Depth][1]);
}

//������������Ĺ�����
void COMPUTER3(){
    int MIN=-INF;
    int BEST_I,BEST_J,BEST_NI,BEST_NJ;
    Copy(map,rec[0]);
    Inform[0][0]=-INF;
    Inform[0][1]=INF;
    //�������(����)��
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            if(map[i][j]==2 && can_move(i,j)){
                for(int k=1;k<=top;k++)
                    Greed[k]=Calcu_Next(i,j,k,0);
                //��̰�������׵÷ֵ�����
                for(int k=1;k<=top;k++)
                    for(int l=1;l<=top-k;l++)
                        if(Greed[l]<Greed[l+1]){
                            int tmp=Greed[l];Greed[l]=Greed[l+1];Greed[l+1]=tmp;
                            tmp=move_list[l];move_list[l]=move_list[l+1];move_list[l+1]=tmp;
                        }

                for(int k=1;k<=top;k++){
                    int nx=i+mvx[move_list[k]],ny=j+mvy[move_list[k]],Interest;
                    Deal_With(i,j,nx,ny,2,map);
                    Inform[1][0]=-INF;
                    Inform[1][1]=INF;

                    Search2(1);
                    Copy(rec[0],map);
                    if(Inform[0][0]>MIN){
                        BEST_NI=nx;BEST_NJ=ny;BEST_I=i;BEST_J=j;
                        MIN=Inform[0][0];
                    }
                }
            }
    }
    if(MIN==-INF){printf("������˼���һ��ˣ�");return;}
    Deal_With(BEST_I,BEST_J,BEST_NI,BEST_NJ,2,map);
    printf("���԰�(%d,%d)�ƶ�����(%d,%d):\n",BEST_I,BEST_J,BEST_NI,BEST_NJ);
}

//������Ϸ����
void operation1(){
    int x1,y1,x2,y2;
    while(true){
        print2(map);
        if(!cnt[0]){
            if(cnt[1]>cnt[2]) printf("��Ӯ��!Orz!");
            else printf("������Ӯ��!Orz!");
            return ;
        }
        int f1=false,f2=false;
        for(int i=1;i<=7;i++)
            for(int j=1;j<=7;j++)
                if(map[i][j]==1 && !f1){
                    if(can_move(i,j)) f1=true;
                }
        if(!f1) {printf("������Ӯ��!Orz!");return;}
        int ord;
        printf("����Ҫ�浵�������Ҫ������1��������0\n");
        scanf("%d",&ord);
        if(ord)    {printf("�Ѵ浵��\n");Save1();return ;}
        bool Over=0;
        while(true){
            printf("����������Ҫ�ƶ������ӵ��к���\n");
            scanf("%d%d",&x1,&y1);
            if(map[x1][y1]!=1)
                printf("���ڸ�λ��û������!\n");
            else if(!can_move(x1,y1)) printf("��ѡ��������޷��ƶ�!\n");
            else{
                while(true){
                    printf("����������Ҫ�ƶ������ӵ���λ�ã�\n");
                    scanf("%d%d",&x2,&y2);
                    if(abs(x2-x1)>2 || abs(y2-y1)>2 || (x1==x2 && y1==y2) || map[x2][y2]){
                        printf("�����ƶ����Ϸ���\n"); break;
                    }
                    else{
                        Over=1;
                        break;
                    }
                }
            }
            if(Over) break;
        }
        Deal_With(x1,y1,x2,y2,1,map);
        print2(map);
        for(int i=1;i<=7;i++)
            for(int j=1;j<=7;j++)
                if(map[i][j]==2 && !f2)
                    if(can_move(i,j)) f2=true;
        if(!f2) {printf("��Ӯ��!Orz!");return;}
        if(!cnt[0]){
            if(cnt[1]>cnt[2]) printf("PLAYER1Ӯ��!Orz!");
            else printf("PLAYER2Ӯ��!Orz!");
            return ;
        }
        COMPUTER3();
    }
}

//˫����Ϸ����
void operation2(){
    int x1,y1,x2,y2;
    while(true){
        bool f1=false,f2=false;
        print2(map);
        for(int i=1;i<=7;i++)
            for(int j=1;j<=7;j++)
                if(map[i][j]==1 && !f1)
                    if(can_move(i,j)) f1=true;
        if(!f1) {printf("PLAYER2Ӯ��!Orz!");return;}
        if(!cnt[0]){
            if(cnt[1]>cnt[2]) printf("PLAYER1Ӯ��!Orz!");
            else printf("PLAYER2Ӯ��!Orz!");
            return ;
        }

        int ord;
        printf("����Ҫ�浵�������Ҫ������1��������0\n");
        scanf("%d",&ord);
        if(ord)    {printf("�Ѵ浵��\n");Save2();return ;}

        bool Over=0;
        while(true){
            printf("������PLAYER1������\n����������Ҫ�ƶ������ӵ��к���\n");
            scanf("%d%d",&x1,&y1);
            if(map[x1][y1]!=1)
                printf("���ڸ�λ��û������!\n");
            else if(!can_move(x1,y1)) printf("��ѡ��������޷��ƶ�!\n");
            else{
                while(true){
                    printf("����������Ҫ�ƶ������ӵ���λ�ã�\n");
                    scanf("%d%d",&x2,&y2);
                    if(abs(x2-x1)>2 || abs(y2-y1)>2 || (x1==x2 && y1==y2) || map[x2][y2]){
                        printf("�����ƶ����Ϸ���\n"); break;
                    }
                    else{
                        Over=1;
                        break;
                    }
                }
            }
            if(Over) break;
        }
        Deal_With(x1,y1,x2,y2,1,map);
        print2(map);
        for(int i=1;i<=7;i++)
            for(int j=1;j<=7;j++)
                if(map[i][j]==2 && !f2)
                    if(can_move(i,j)) f2=true;
        if(!f2) {printf("PLAYER1Ӯ��!Orz!");return;}
        if(!cnt[0]){
            if(cnt[1]>cnt[2]) printf("PLAYER1Ӯ��!Orz!");
            else printf("PLAYER2Ӯ��!Orz!");
            return ;
        }

        Over=0;
        while(true){
            printf("������PLAYER2������\n����������Ҫ�ƶ������ӵ��к���\n");
            scanf("%d%d",&x1,&y1);
            if(map[x1][y1]!=2)
                printf("���ڸ�λ��û������!\n");
            else if(!can_move(x1,y1)) printf("��ѡ��������޷��ƶ�!\n");
            else{
                while(true){
                    printf("����������Ҫ�ƶ������ӵ���λ�ã�\n");
                    scanf("%d%d",&x2,&y2);
                    if(abs(x2-x1)>2 || abs(y2-y1)>2 || (x1==x2 && y1==y2) || map[x2][y2]){
                        printf("�����ƶ����Ϸ���\n"); break;
                    }
                    else{
                        Over=1;
                        break;
                    }
                }
            }
            if(Over) break;
        }
        Deal_With(x1,y1,x2,y2,2,map);

    }
}
