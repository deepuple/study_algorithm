//시간 초과
//H2106
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT		100
#define CMD_ADD			200
#define CMD_MOVETYPE	300
#define CMD_MOVENAME	400
#define CMD_DELETENAME	500
#define CMD_COUNTBOOK	600

#define MAX_N			5
#define MAX_NAME_LEN	7
#define MAX_TAG_LEN		4
#define MOD 			4095

int bufCnt;
int secCnt[101];

void mstrcpy(char dst[], const char src[]) {
    int c = 0;
    while ((dst[c] = src[c]) != '\0') ++c;
}

int mstrcmp(const char str1[], const char str2[]) {
    int c = 0;
    while (str1[c] != '\0' && str1[c] == str2[c]) ++c;
    return str1[c] - str2[c];
}

struct Book{
    char name[MAX_NAME_LEN];
    char type[MAX_N][MAX_TAG_LEN] ;
    int section;
    int typeCnt;
    Book* hash_np;
    Book* hash_assign(const char* str, int typeNum, char types[MAX_N][MAX_TAG_LEN], int sec, Book* p){
        mstrcpy(name, str);
        for (int i = 0; i < typeNum; i++){
            mstrcpy(type[i], types[i]);
        }
        typeCnt = typeNum;
        section = sec;
        hash_np = p;
        return this;
    }
    Book* list_next;
    Book* list_prev;
    Book* list_assign(Book* p){
        list_next = p;
        if(p)
            p->list_prev = this;
        return this;
    }
}buf[100001], *hTbl[MOD+1], *head[101];

unsigned long myhash(const char *str){
    unsigned long hash = 5381;
    int c;
    while (c = *str++)	{
        hash = (((hash << 5) + hash) + c) & MOD;
    }
    return hash % MOD;
}

Book* findBookWithName(const char *str){
    unsigned long h = myhash(str);
    Book* ret = hTbl[h];
    while (ret && mstrcmp(ret->name, str)!=0){
        ret = ret->hash_np;
    }
    return ret;
}

void addBookInsec(Book* nb, int sec){
    nb->list_next = head[sec];
    nb->list_prev = NULL;
    if(head[sec])
        head[sec]->list_prev = nb;
    nb->section = sec;
    head[sec] = nb;
    secCnt[sec]++;
}

int deleteBook(Book* p){
    int sec = p->section;
    if(p){
        if(p->list_prev)
            p->list_prev->list_next = p->list_next;
        else //head
            head[sec] = p->list_next;
            
        if(p->list_next)
            p->list_next->list_prev = NULL;

        p->list_next = NULL;
        p->list_prev = NULL;
        secCnt[sec]--;
        return 1;
    }
    return 0;
}

int deleteBookInSecWithName(int sec, char mName[]){
    Book* it = head[sec];

    while(it){
        if(mstrcmp(it->name, mName)==0){//head
            head[sec] = it->list_next;
            secCnt[sec]--;
            it->list_next = NULL;
            it = head[sec];
            return 1;
        }else if(it->list_next && mstrcmp(it->list_next->name, mName)==0){
            Book* target = it->list_next;
            it->list_next = target->list_next;
            target->list_next = NULL;
            secCnt[sec]--;
            return 1;
        }else
            it = it->list_next;
    }
    return 0;
}

void init(int M){
    bufCnt = 0;
    for(int i = 0; i < MOD+1; i++)        
        hTbl[i] = NULL;
    
    for(int i = 1; i <= M; i++){        
        head[i] = NULL;
        secCnt[i] = 0;
    }
}

void add(char mName[MAX_NAME_LEN], int mTypeNum, char mTypes[MAX_N][MAX_TAG_LEN], int mSection){
    unsigned long idx = myhash(mName);
    hTbl[idx] = buf[bufCnt++].hash_assign(mName, mTypeNum, mTypes, mSection, hTbl[idx]);
    head[mSection] = hTbl[idx]->list_assign(head[mSection]);
    secCnt[mSection]++;
}

bool isTypeFound(Book* p, char mType[MAX_TAG_LEN]){
    for(int i = 0 ; i < p->typeCnt; i++){
        if(mstrcmp(p->type[i], mType)==0)
            return true;
    }
    return false;
}

int moveType(char mType[MAX_TAG_LEN], int mFrom, int mTo){
    Book* it = head[mFrom];
    int movCnt=0;
    while(it){
        if(isTypeFound(it, mType)){ //head
            head[mFrom] = it->list_next;
            secCnt[mFrom]--;
            addBookInsec(it, mTo);
            it = head[mFrom];
            movCnt++;
        }else if(it->list_next && isTypeFound(it->list_next, mType)){
            Book* tmp = it->list_next;
            it->list_next = tmp->list_next;
            secCnt[mFrom]--;
            addBookInsec(tmp, mTo);
            movCnt++;
        }else
            it = it->list_next;
    }
    return movCnt;
}

void moveName(char mName[MAX_NAME_LEN], int mSection){
    Book* b = findBookWithName(mName);
    //deleteBookInSecWithName(b->section, mName);
    deleteBook(b);
    addBookInsec(b, mSection);
}

void deleteName(char mName[MAX_NAME_LEN]){
    Book* b = findBookWithName(mName);
    deleteBook(b);
    //deleteBookInSecWithName(b->section, mName);
}

bool isFound(int typeCnt, char mType[MAX_N][MAX_TAG_LEN], int mTypeNum, char mTypes[MAX_N][MAX_TAG_LEN]){
    for(int i = 0 ; i < typeCnt ; i++){
        for(int j = 0 ; j < mTypeNum; j++){
            if(mstrcmp(mType[i], mTypes[j])==0){
                return true;
            }
        }
    }
    return false;
}

int countBook(int mTypeNum, char mTypes[MAX_N][MAX_TAG_LEN], int mSection){
    Book* it = head[mSection];	
    int cnt = 0;

    while (it){
        if(isFound(it->typeCnt, it->type, mTypeNum, mTypes))
            cnt++;
        it = it->list_next;
    }
    return cnt;
}

static bool run()
{
    int  Q;
    int  cmd, M, mTypeNum, mSection, mFrom, mTo;
    char mName[MAX_NAME_LEN], mType[MAX_TAG_LEN], mTypes[MAX_N][MAX_TAG_LEN];
    
    int  ret, ans;
    
    scanf("%d", &Q);
        
    bool okay = false;
    
    for (int q = 0; q <= Q; ++q)
    {
        scanf("%d", &cmd);

        switch(cmd)
        {
        case CMD_INIT:
            scanf("%d", &M);
            init(M);
            okay = true;
            break;
        case CMD_ADD:
            scanf("%s %d", mName, &mTypeNum);
            for (int i = 0; i < mTypeNum; ++i)
                scanf("%s", mTypes[i]);
            scanf("%d", &mSection);
            if (okay)
                add(mName, mTypeNum, mTypes, mSection);
            break;
        case CMD_MOVETYPE:
            scanf("%s %d %d", mType, &mFrom, &mTo);
            if (okay)
                ret = moveType(mType, mFrom, mTo);
            scanf("%d", &ans);
            if (ret != ans)
                okay = false;
            break;
        case CMD_MOVENAME:
            scanf("%s %d", mName, &mSection);
            if (okay)
                moveName(mName, mSection);
            break;
        case CMD_DELETENAME:
            scanf("%s", mName);
            if (okay)
                deleteName(mName);
            break;
        case CMD_COUNTBOOK:
            scanf("%d", &mTypeNum);
            for (int i = 0; i < mTypeNum; ++i)
                scanf("%s", mTypes[i]);
            scanf("%d", &mSection);
            if (okay)
                ret = countBook(mTypeNum, mTypes, mSection);
            scanf("%d", &ans);
            if (ret != ans)
                okay = false;
            break;
        }
    }
    
    return okay;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("H2106_input.txt", "r", stdin);

    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        
        printf("#%d %d\n", tc, score);
    }

    return 0;
}
