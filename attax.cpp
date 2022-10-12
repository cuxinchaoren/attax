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

void init();//初始化
void menu();//操作界面
void sum();//统计棋子数目
void print(int map[][9]);//输出棋盘
void print2(int map[][9]);//输出一个好看的棋盘
void operation1();//单人游戏
void operation2();//双人游戏
void Copy();
void Save1();//存单人游戏档
void Save2();//存双人游戏档
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
    printf(" ━━━━━━━━━━━━━━━━━━━━━━━━ \n");
    printf("欢迎来到同化棋小游戏，您可以输入数字来进行以下操作\n");
    printf(" ━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("    0.退出游戏\n");
    printf("    1.单人游戏\n");
    printf("    2.双人游戏\n");
    scanf("%d",&ord1);
    if(ord1==1){
        printf("     ━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("    您来到了单人游戏，您可以输入数字来进行以下操作\n");
        printf("     ━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("        0.返回上层\n");
        printf("        1.新游戏\n");
        printf("        2.读取存档\n");
        scanf("%d",&ord2);
        if(ord2==0) menu();//返回上层即再次递归
        else{
            init();
            if(ord2==2){//读取存档
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
        printf("     ━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("    您来到了双人游戏，您可以输入数字来进行以下操作\n");
        printf("     ━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("        0.返回上层\n");
        printf("        1.新游戏\n");
        printf("        2.读取存档\n");
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

//初始化函数
void init(){
    //初始化棋盘
    map[1][7]=map[7][1]=1;
    map[1][1]=map[7][7]=2;
    //初始化移动和周围数组
    int tmp=0;
    for(int i=-2;i<=2;i++)
        for(int j=-2;j<=2;j++)
            if(!(i==0 && j==0)) tmp++,mvx[tmp]=i,mvy[tmp]=j; //mvx和mvy分别表示棋子移动时x方向上和y方向上的移动
    tmp=0;
    for(int i=-1;i<=1;i++)
        for(int j=-1;j<=1;j++)
            if(!(i==0 && j==0)) tmp++,arx[tmp]=i,ary[tmp]=j; //arx和ary分别表示染色时在x和y上的移动
}

//统计棋盘内各种子的数目
void sum(int map[][9]){
    cnt[0]=cnt[1]=cnt[2]=0;
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++)
            cnt[map[i][j]]++;
}

//输出一个简陋的棋盘(@和#组成)
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

//输出一个漂亮的棋盘，使用制表符
void print2(int map[][9]){
    sum(map);
    printf(" ━━━━━━━━━━━━━━━━ \n");
    printf(" YOU: %2d      ┃COM: %2d\n",cnt[1],cnt[2]);
    printf(" ━━━━━━━━━━━━━━━━ \n");
    printf("     1   2   3   4   5   6   7  \n");
    printf("   ┏━━┳━━┳━━┳━━┳━━┳━━┳━━┓\n");
    printf(" 1 ┃");
    for(int i=1;i<=7;i++)
        if(map[1][i]==1) printf("●┃");
        else if(map[1][i]==2) printf("○┃");
        else printf("  ┃");
    putchar('\n');
    for(int i=2;i<=7;i++){
        printf("   ┣━━╋━━╋━━╋━━╋━━╋━━╋━━┫\n");
        printf(" %d ┃",i);
        for(int j=1;j<=7;j++)
            if(map[i][j]==1) printf("●┃");
            else if(map[i][j]==2) printf("○┃");
            else printf("  ┃");
        putchar('\n');
    }
    printf("   ┗━━┻━━┻━━┻━━┻━━┻━━┻━━┛\n");
}

//判断一个位置的琪是否还有位置可以移动 ，同时将可以移动的方向存在move_list中
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

//处理一个从(x1,y1)移动到(x2,y2)的t种棋子所产生的效果
void Deal_With(int x1,int y1,int x2,int y2,int t,int map[][9]){
    map[x1][y1]=t*(abs(x2-x1)<=1 && abs(y2-y1)<=1);
    map[x2][y2]=t;
    int nx,ny;
    for(int i=1;i<=8;i++){
        nx=x2+arx[i],ny=y2+ary[i];
        if(map[nx][ny]) map[nx][ny]=t;
    }
}

//单人游戏存档
void Save1(){
    FILE *fp;
    fp=fopen("rec1.txt","w");
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            fprintf(fp,"%d ",map[i][j]);
        fprintf(fp,"\n");
    }
}

//双人游戏存档
void Save2(){
    FILE *fp;
    fp=fopen("rec2.txt","w");
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            fprintf(fp,"%d ",map[i][j]);
        fprintf(fp,"\n");
    }
}

//复制两个棋盘
void Copy(int A[][9],int B[][9]){
    for(int i=1;i<=7;i++)
        for(int j=1;j<=7;j++)
            B[i][j]=A[i][j];
}

//电脑0，使用黑棋子最多战略
void COMPUTER0(){
    printf("现在是电脑0时间！");
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
    printf("电脑把(%d,%d)移动到了(%d,%d):\n",BEST_I,BEST_J,BEST_NI,BEST_NJ);
}

//电脑1，使用黑棋减去白琪最多战略
void COMPUTER1(){
    printf("现在是电脑1时间！");
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
    printf("电脑把(%d,%d)移动到了(%d,%d):\n",BEST_I,BEST_J,BEST_NI,BEST_NJ);
}

int height[9],ins[9],ht;//height和ht表示维护的单调栈和栈顶指针，ins表示每个位置能管到的最前面的位置
int Left[9][9];//Left表示统计这个位置往左边最长延伸为多少

//返回两个值中的最大值
int Max(int x,int y){
    return x>y?x:y;
}

int Min(int x,int y){
    return x<y?x:y;
}

//计算map中t的最大矩阵，使用单调栈
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

//COM1优化：当黑子减白子相同时，可以通过最大矩阵进行第二次判断
void COMPUTER2(){
    printf("现在是电脑2时间！");
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
    printf("电脑把(%d,%d)移动到了(%d,%d):\n",BEST_I,BEST_J,BEST_NI,BEST_NJ);
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

//假设电脑是得分者
void Search1(int Depth){
    if(Depth>3){
        Inform[Depth-1][1]=Min(Inform[Depth-1][1],Judge(map));
        return ;
    }
    Copy(map,rec[Depth]);
    //如果是我(电脑)下
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            if(map[i][j]==2 && can_move2(i,j,Depth)){
                for(int k=1;k<=TP[Depth];k++)
                    Greed[k]=Calcu_Next(i,j,k,Depth);
                //把贪心中容易得分的先下
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
    //如果是阻碍者下
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            if(map[i][j]==1 && can_move2(i,j,Depth)){
                for(int k=1;k<=TP[Depth];k++)
                    Greed[k]=Calcu_Next(i,j,k,Depth);
                //把贪心中难以得分的先下
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

//这个就是搜索的过程了
void COMPUTER3(){
    int MIN=-INF;
    int BEST_I,BEST_J,BEST_NI,BEST_NJ;
    Copy(map,rec[0]);
    Inform[0][0]=-INF;
    Inform[0][1]=INF;
    //如果是我(电脑)下
    for(int i=1;i<=7;i++){
        for(int j=1;j<=7;j++)
            if(map[i][j]==2 && can_move(i,j)){
                for(int k=1;k<=top;k++)
                    Greed[k]=Calcu_Next(i,j,k,0);
                //把贪心中容易得分的先下
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
    if(MIN==-INF){printf("不好意思它挂机了！");return;}
    Deal_With(BEST_I,BEST_J,BEST_NI,BEST_NJ,2,map);
    printf("电脑把(%d,%d)移动到了(%d,%d):\n",BEST_I,BEST_J,BEST_NI,BEST_NJ);
}

//单人游戏操作
void operation1(){
    int x1,y1,x2,y2;
    while(true){
        print2(map);
        if(!cnt[0]){
            if(cnt[1]>cnt[2]) printf("您赢了!Orz!");
            else printf("您差点就赢了!Orz!");
            return ;
        }
        int f1=false,f2=false;
        for(int i=1;i<=7;i++)
            for(int j=1;j<=7;j++)
                if(map[i][j]==1 && !f1){
                    if(can_move(i,j)) f1=true;
                }
        if(!f1) {printf("您差点就赢了!Orz!");return;}
        int ord;
        printf("您需要存档吗？如果需要请输入1否则输入0\n");
        scanf("%d",&ord);
        if(ord)    {printf("已存档！\n");Save1();return ;}
        bool Over=0;
        while(true){
            printf("请输入您需要移动的棋子的行和列\n");
            scanf("%d%d",&x1,&y1);
            if(map[x1][y1]!=1)
                printf("您在该位置没有棋子!\n");
            else if(!can_move(x1,y1)) printf("您选择的棋子无法移动!\n");
            else{
                while(true){
                    printf("请输入您需要移动的棋子到的位置：\n");
                    scanf("%d%d",&x2,&y2);
                    if(abs(x2-x1)>2 || abs(y2-y1)>2 || (x1==x2 && y1==y2) || map[x2][y2]){
                        printf("您的移动不合法！\n"); break;
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
        if(!f2) {printf("您赢了!Orz!");return;}
        if(!cnt[0]){
            if(cnt[1]>cnt[2]) printf("PLAYER1赢了!Orz!");
            else printf("PLAYER2赢了!Orz!");
            return ;
        }
        COMPUTER3();
    }
}

//双人游戏操作
void operation2(){
    int x1,y1,x2,y2;
    while(true){
        bool f1=false,f2=false;
        print2(map);
        for(int i=1;i<=7;i++)
            for(int j=1;j<=7;j++)
                if(map[i][j]==1 && !f1)
                    if(can_move(i,j)) f1=true;
        if(!f1) {printf("PLAYER2赢了!Orz!");return;}
        if(!cnt[0]){
            if(cnt[1]>cnt[2]) printf("PLAYER1赢了!Orz!");
            else printf("PLAYER2赢了!Orz!");
            return ;
        }

        int ord;
        printf("您需要存档吗？如果需要请输入1否则输入0\n");
        scanf("%d",&ord);
        if(ord)    {printf("已存档！\n");Save2();return ;}

        bool Over=0;
        while(true){
            printf("现在是PLAYER1操作！\n请输入您需要移动的棋子的行和列\n");
            scanf("%d%d",&x1,&y1);
            if(map[x1][y1]!=1)
                printf("您在该位置没有棋子!\n");
            else if(!can_move(x1,y1)) printf("您选择的棋子无法移动!\n");
            else{
                while(true){
                    printf("请输入您需要移动的棋子到的位置：\n");
                    scanf("%d%d",&x2,&y2);
                    if(abs(x2-x1)>2 || abs(y2-y1)>2 || (x1==x2 && y1==y2) || map[x2][y2]){
                        printf("您的移动不合法！\n"); break;
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
        if(!f2) {printf("PLAYER1赢了!Orz!");return;}
        if(!cnt[0]){
            if(cnt[1]>cnt[2]) printf("PLAYER1赢了!Orz!");
            else printf("PLAYER2赢了!Orz!");
            return ;
        }

        Over=0;
        while(true){
            printf("现在是PLAYER2操作！\n请输入您需要移动的棋子的行和列\n");
            scanf("%d%d",&x1,&y1);
            if(map[x1][y1]!=2)
                printf("您在该位置没有棋子!\n");
            else if(!can_move(x1,y1)) printf("您选择的棋子无法移动!\n");
            else{
                while(true){
                    printf("请输入您需要移动的棋子到的位置：\n");
                    scanf("%d%d",&x2,&y2);
                    if(abs(x2-x1)>2 || abs(y2-y1)>2 || (x1==x2 && y1==y2) || map[x2][y2]){
                        printf("您的移动不合法！\n"); break;
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
