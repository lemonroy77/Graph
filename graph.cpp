#include <stdio.h>
#include <stdlib.h>
#define INF 32767
#define MAXV 100 //定义最大的顶点个数
typedef char InfoType;
typedef int ElemType;
#define MaxSize 100
using namespace std;
typedef char VertexType;
//邻接矩阵
typedef struct{
    int edges[MAXV][MAXV];//邻接矩阵，边表
    VertexType vex[MAXV];//存放头结点的顺序表
    int n,e; //顶点数，边数
}MatGraph;
//邻接表
typedef struct ArcNode{//边节点
    int adjvex;//邻接顶点编号
    struct ArcNode *nextarc;
    int weight;
}ArcNode;
typedef struct VNode{//头节点
    InfoType info;
    ArcNode*firstarc;
}VNode;
typedef struct{
    VNode adjlist[MAXV];//存放头节点的顺序表
    int n,e;
}AdjGraph;
//循环队列
typedef struct{
    ElemType data[MaxSize];
    int front,rear;
}SeQueue;
void InitQueue(SeQueue &Q){
    Q.front=Q.rear=0;
}
bool QueueEmpty(SeQueue Q){
    if(Q.front==Q.rear)  return true;
    return false;
}
bool QueueFull(SeQueue Q){
    if((Q.rear+1)%MaxSize==Q.front) return true;
    return false;
}
bool EnQueue(SeQueue&Q,ElemType x){
    if(QueueFull(Q)) return false;
    Q.data[Q.rear]=x;
    Q.rear=(Q.rear+1)%MaxSize;
    return true;
}
bool DeQueue(SeQueue &Q,ElemType&x){
    if(QueueEmpty(Q)) return false;
    x=Q.data[Q.front];
    Q.front=(Q.front+1)%MaxSize;
    return true;
}
bool GetHead(SeQueue Q, ElemType& x) {
    if (QueueEmpty(Q))
        return false;
    x = Q.data[Q.front];
    return true;
}
//邻接矩阵
void CreateMat(MatGraph &g,int A[MAXV][MAXV],int n,int e){
    int i,j;
    g.n=n;g.e=e;
    for(int i=0;i<g.n;i++)
        for(int j=0;j<g.n;j++)
            g.edges[i][j]=A[i][j];
}
void printmat(MatGraph g){
    printf("邻接矩阵为：\n");
    for(int i=0;i<g.n;i++){
        for(int j=0;j<g.n;j++){
            if(g.edges[i][j]!=INF)
                printf("%4d",g.edges[i][j]);
            else
                printf("%4s","∞");
        }
        printf("\n");
    }
}
void CreateAdj(AdjGraph *&G,int A[MAXV][MAXV],int n,int e){
    ArcNode *p;
    G=(AdjGraph*)malloc(sizeof(AdjGraph));
    G->n=n;G->e=e;
    for(int i=0;i<n;i++)
        G->adjlist[i].firstarc=NULL;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(A[i][j]!=INF&&A[i][j]!=0){
                p=(ArcNode*)malloc(sizeof(ArcNode));
                p->adjvex=j;
                p->weight=A[i][j];
                p->nextarc=G->adjlist[i].firstarc;
                G->adjlist[i].firstarc=p;
            }
};
void printadj(AdjGraph*G){
    ArcNode*p;
    for(int i=0;i<G->n;i++){
        p=G->adjlist[i].firstarc;
        printf("%3d:",i);
        while(p){
            printf("%3d[%d]->",p->adjvex,p->weight);
            p=p->nextarc;
        }
        printf("^\n");
    }
}
int visited[MAXV]={0};
void DFS(AdjGraph *G,int v){
    visited[v]=1;
    printf("%d ",v);
    ArcNode*p=G->adjlist[v].firstarc;
    while(p){
      if(visited[p->adjvex]==0)
        DFS(G,p->adjvex);
        p=p->nextarc;
    }
}
int Visited[MAXV]={0};	  //全局变量，所有元素置初值0
void MDFS(MatGraph g,int v)//根据图的邻接矩阵深度优先遍历
{      int w;
       printf("%d ",v);	  //访问顶点v
       Visited[v]=1;		  //置访问标记
       for (w=0;w<g.n;w++)	  //找顶点v的所有相邻点
       if (g.edges[v][w]!=0 && g.edges[v][w]!=INF && Visited[w]==0)
           MDFS(g,w);  //找顶点v的未访问过的相邻点w
}
void BFS(AdjGraph*G,int v){
    SeQueue Q;
    InitQueue(Q);
    EnQueue(Q,v);
    int w;
    int visited[G->n];
    for(int i=0;i<G->n;i++)
        visited[i]=0;
    ArcNode*p;
    printf("%d ",v);
    visited[v]=1;
    while(!QueueEmpty(Q)){
        DeQueue(Q, w);
        p = G->adjlist[w].firstarc; //遍历当前节点的指针
        while(p!= NULL){
            if(visited[p->adjvex] == 0) //还未访问过
            {
//d[p->adjvex] = d[w]+1;path[p->adjvex] = path[w]; //BFS解决单源最短路径
                EnQueue(Q, p->adjvex);
                visited[p->adjvex] = 1;
                printf("%d ",p->adjvex);
            }
            p = p->nextarc;
        }
    }
    printf("\n");
}
typedef struct{
    int data[MaxSize];
    int top;
}SeqStack;
void InitStack(SeqStack &s){
    s.top=-1;
}
void push(SeqStack &s,int x){
    if(s.top==MaxSize-1)   return ;
    s.top++;
    s.data[s.top]=x;
}
void pop(SeqStack &s,int &x){
    if(s.top==-1) return ;
    x=s.data[s.top];
    s.top--;
}
void GetTop(SeqStack s,int &x){
    x=s.data[s.top];
}
bool IsEmpty(SeqStack s){
    if(s.top==-1) return true;
    return false;
}
int print[MAXV];
int indegree[MAXV] = {0,1,3,1,0};
bool topsort(AdjGraph *G){
    SeqStack s;
    InitStack(s); //初始化栈
    int i; ArcNode *p;
    for(i = 0; i < G->n; i++){
        if(indegree[i] == 0){
            push(s, i);
        }
    }
    int count = 0; //计数
    while(!IsEmpty(s)){
        pop(s, i);
        print[count++] = i;
        for(p = G->adjlist[i].firstarc; p; p = p->nextarc){
            int v = p->adjvex;
            if((--indegree[v]) == 0)
                push(s, v);
        }
    }
    if(count < G->n)
        return false;
    return true;
}
typedef struct{
    int u;
    int v;
    int w;
}Edge;
void InsortEdge(Edge a[],int n){
    int i,j;
    for(int i=1;i<n;i++){
        Edge temp;
        temp.w=a[i].w;
        temp.u=a[i].u;
        temp.v=a[i].v;
        for(j=i;j>0;j--){
            a[j].w=a[j-1].w;
            a[j].u=a[j-1].u;
            a[j].v=a[j-1].v;
        }
        a[j].w=temp.w;
        a[j].u=temp.u;
        a[j].v=temp.v;
    }
    return;
}
void Kruskal(MatGraph g){
    int i,j,k=0,u1,v1,sn1,sn2;
    Edge E[MaxSize];
    int vset[MAXV];
    for(i=0;i<g.n;i++)
        for(j=0;j<g.n;j++)
            if(g.edges[i][j]!=0&&g.edges[i][j]!=INF){
                E[k].u=i;
                E[k].v=j;
                E[k].w=g.edges[i][j];
                k++;
            }
    InsortEdge(E,g.e);
    for(i=0;i<g.n;i++)
        vset[i]=i;
    k=1;j=0;
    while(k<g.n){
        u1=E[j].u;
        v1=E[j].v;
        sn1=vset[u1];
        sn2=vset[v1];
        if(sn1!=sn2){
            printf("(%d,%d) :%d\n",u1,v1,E[j].w);
            k++;
            vset[v1]=sn1;
        }
        j++;
    }
}
int main() {
    MatGraph g;
     int A[MAXV][MAXV] = {
            {0,   4,   3, INF, INF, INF, INF, INF},
            {4,   0,   5,   5, 9,   INF, INF, INF},
            {3,   5,   0,   5, INF, INF, INF, 5},
            {INF, 5,   5,   0, 7,   6,   5,   4},
            {INF, 9,   INF, 7, 0,   3,   INF, INF},
            {INF, INF, INF, 6, 3,   0,   2,   INF},
            {INF, INF, INF, 5, INF, 2,   0,   6},
            {INF, INF, 5,   4, INF, INF, 6,   0}};
    int n = 8, e = 14;
 /*  int A[MAXV][MAXV]={{0,1,0,1,0},
                       {0,0,1,0,0},
                       {0,0,0,0,0},
                       {0,0,1,0,0},
                       {0,0,1,0,0}};
    int n=5, e=5;*/
    CreateMat(g,A,n,e);
    printmat(g);
    AdjGraph*G;
    CreateAdj(G,A,n,e);
    printf("图G的邻接表:\n");
    printadj(G);
    printf("深度优先遍历\n");
    printf("根据图的邻接表深度优先遍历：\n");
    DFS(G,0);
    printf("\n根据图的邻接矩阵深度优先遍历：\n");
    MDFS(g,0);
    printf("\n广度优先遍历\n");
    BFS(G,0);
  /*  printf("topsort:\n");
   topsort(G);
    for(int i=0;i<G->n;i++){
        printf("%d ",print[i]);
    }*/
  printf("\nKruskal\n");
  Kruskal(g);
    return 0;
}