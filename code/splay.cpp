//POJ3580
//SPLAY tree 
//support add by segment,reverse,delete,insert.
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

#define Key_value ch[ch[root][1] ][0]

const int maxn = 5e5+10;
const int INF = 0x3f3f3f3f;

int pre[maxn],ch[maxn][2],key[maxn],sz[maxn];
int root,tot1;
int rev[maxn],mi[maxn],add[maxn];
int s[maxn],tot2;
int a[maxn];
int n,q;

void Treavel(int x)
{
    if(x)
    {
        Treavel(ch[x][0]);
        printf("结点:%2d: 左儿子 %2d 右儿子 %2d 父结点 %2d key=%2d size= %2d mi=%2d add=%2d\n",x,ch[x][0],ch[x][1],pre[x],key[x],sz[x],mi[x],add[x]);
        Treavel(ch[x][1]);
    }
}
void debug()
{
    printf("root:%d\n",root);
    Treavel(root);
}
//

void NewNode(int &r,int father,int k)
{
    if(tot2) r = s[tot2--];
    else r = ++tot1;
    pre[r] = father;
    ch[r][0] = ch[r][1] = 0;
    key[r] = k;
    mi[r] = k;
    rev[r] = add[r] = 0;
    sz[r] = 1;
}

void Update_add(int r,int c)
{
    if(!r) return ;
    key[r] += c;
    mi[r] += c;
    add[r] += c;
}

void Update_rev(int r)
{
    if(!r) return ;
    swap(ch[r][0],ch[r][1]);
    rev[r] ^= 1;
}

void push_up(int r)
{
    int lson = ch[r][0],rson = ch[r][1];
    sz[r] = sz[lson] + sz[rson] + 1;
    mi[r] = min(min(mi[lson],mi[rson]),key[r]);
}

void push_down(int r)
{
    if(rev[r])
    {
        Update_rev(ch[r][0]);
        Update_rev(ch[r][1]);
        rev[r] = 0;
    }
    if(add[r])
    {
        Update_add(ch[r][0],add[r]);
        Update_add(ch[r][1],add[r]);
        add[r] = 0;
    }
}

void Build(int &x,int l,int r,int father)
{
    if(l>r) return ;
    int mid = (l+r)>>1;
    NewNode(x,father,a[mid]);
    Build(ch[x][0],l,mid-1,x);
    Build(ch[x][1],mid+1,r,x);
    push_up(x);
}

void Init()
{
    root = tot1 = tot2 = 0;
    ch[root][0] = ch[root][1] = sz[root] = pre[root] = 0;
    rev[root] = key[root] = 0;
    mi[root] = INF;
    NewNode(root,0,0);
    NewNode(ch[root][1],root,0);
    for(int i=1;i<=n;i++) scanf("%d",&a[i]);
    Build(Key_value,1,n,ch[root][1]);
    push_up(ch[root][1]);
    push_up(root);
}
void Rotate(int x,int kind)
{
    int y = pre[x];
    push_down(y);
    push_down(x);
    ch[y][!kind] = ch[x][kind];
    pre[ch[x][kind] ] = y;
    if(pre[y])
        ch[pre[y] ][ch[pre[y]][1]==y ] = x;
    pre[x] = pre[y];
    ch[x][kind] = y;
    pre[y] = x;
    push_up(y);
}
void Splay(int r,int goal)
{
    push_down(r);
    while(pre[r] != goal)
    {
        if(pre[pre[r] ] == goal)
        {
            push_down(pre[r]);
            push_down(r);
            Rotate(r,ch[pre[r]][0] == r);
        }
        else
        {
            push_down(pre[pre[r] ]);
            push_down(pre[r]);
            push_down(r);
            int y = pre[r];
            int kind = ch[pre[y] ][0] == y;
            if(ch[y][kind] == r)
            {
                Rotate(r,!kind);
                Rotate(r,kind);
            }
            else
            {
                Rotate(y,kind);
                Rotate(r,kind);
            }
        }
        push_up(r);
        if(goal == 0) root = r;
    }
}

int Get_kth(int r,int k)
{
    push_down(r);
    int t = sz[ch[r][0] ] + 1;
    if(t == k) return r;
    if(t > k) return Get_kth(ch[r][0],k);
    else return Get_kth(ch[r][1],k-t);
}

void Insert(int pos,int tot)
{
    for(int i=0;i<tot;i++) scanf("%d",&a[i]);
    Splay(Get_kth(root,pos+1) , 0);
    Splay(Get_kth(root,pos+2) , root);
    Build(Key_value,0,tot-1,ch[root][1]);
    push_up(ch[root][1]);
    push_up(root);
}
void erase(int r)
{
    if(!r) return ;
    s[++tot2] = r;
    erase(ch[r][0]);
    erase(ch[r][1]);
}
void Delete(int pos,int tot)
{
    Splay(Get_kth(root,pos) ,0);
    Splay(Get_kth(root,pos+tot+1) , root);
    erase(Key_value);
    pre[Key_value] = 0;
    Key_value = 0;
    push_up(ch[root][1]);
    push_up(root);
}

void Reverse(int pos,int tot)
{
    Splay(Get_kth(root,pos) , 0);
    Splay(Get_kth(root,pos+tot+1), root);
    Update_rev(Key_value);
}

void Add(int pos,int tot,int c)
{
    Splay(Get_kth(root,pos) , 0);
    Splay(Get_kth(root,pos+tot+1) , root);
    Update_add(Key_value,c);
    push_up(ch[root][1]);
    push_up(root);
}

int Get_min(int pos,int tot)
{
    Splay(Get_kth(root,pos) , 0);
    Splay(Get_kth(root,pos+tot+1) , root);
    return mi[Key_value];
}

void Revolve(int l,int r,int t)
{
    if(!t) return ;
    int c = r - t;
    Splay(Get_kth(root,l) , 0);
    Splay(Get_kth(root,c+2),root);
    int tmp = Key_value;
    Key_value = 0;
    push_up(ch[root][1]);
    push_up(root);
    Splay(Get_kth(root,r-c+l) , 0);
    Splay(Get_kth(root,r-c+l+1) , root);
    Key_value = tmp;
    pre[Key_value] = ch[root][1];
    push_up(ch[root][1]);
    push_up(root);
}

int m;
int main()
{
    while(~scanf("%d ",&n))
    {
        Init();
        //debug();
        scanf("%d ",&m);
        char op[10];
        for(int i=0;i<m;i++)
        {
            scanf(" %s",op);
            //printf("i:%d op:%s\n",i,op);
            int x,y,c,t;
            if(op[0] == 'A')            //add
            {
                scanf("%d%d%d",&x,&y,&c);
                Add(x,y-x+1,c);
            }
            else if(op[0] == 'I')       //insert
            {
                scanf("%d",&x);
                Insert(x,1);
            }
            else if(op[0] == 'D')       //delete
            {
                scanf("%d",&x);
                Delete(x,1);
            }
            else if(op[0] == 'M')       //min
            {
                scanf("%d%d",&x,&y);
                printf("%d\n",Get_min(x,y-x+1));
            }
            else if(op[0] == 'R' && op[3] == 'E')//reverse
            {
                scanf("%d%d",&x,&y);
                Reverse(x,y-x+1);
            }
            else                        //revolve
            {
                scanf("%d%d%d",&x,&y,&t);
                t = (t%(y-x+1)+(y-x+1))%(y-x+1);
                Revolve(x,y,t);
            }
            //debug();
        }
    }
}
