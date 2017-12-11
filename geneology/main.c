#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 30+1
typedef enum {M,F} sex;
typedef struct node{
    char name[MAX];
    char last[MAX];
    int age;
    sex sex;
    struct node *next;
    struct node *mother;
    struct node *father;
    struct node *children;
    struct node *nextsex;
    struct node *nextage;
}node_t;
node_t *readF(char *,int *);
node_t set2Null(node_t);
node_t *search(node_t *,char *,char *,int);
node_t *assignparent(node_t *,node_t *, node_t *);
node_t *assignchild(node_t*,node_t *);
int pathsex(node_t *,sex,int);
void writebysex(node_t *);
int pathage(node_t *,int);
void writebyage(node_t *);

int main(int argc,char *argv[])
{
    node_t *person,*sexpath,*agepath;
    int nppl,i,max,j;
    if(argc!=2){
        printf("error in command line args!");
        exit(0);
    }
    person=readF(argv[1],&nppl);

    printf("BY M SEX:\n");
    for(max=0,i=0;i<nppl;i++){
        if(person[i].sex!=M) continue;
        j=pathsex(&person[i],M,0);
        if(j>max){
            max=j;
            sexpath=&person[i];
        }
    }
    writebysex(sexpath);
    for(i=0;i<nppl;i++){
        person[i].nextsex=NULL;
    }
    printf("\n\n");
    printf("BY F SEX:\n");
    for(max=0,i=0;i<nppl;i++){
        if(person[i].sex!=F) continue;
        j=pathsex(&person[i],F,0);
        if(j>max){
            max=j;
            sexpath=&person[i];
        }
    }
    writebysex(sexpath);
    printf("\n\n");
    printf("BY AGE:\n");
    for(i=0,max=0;i<nppl;i++){
        j=pathage(&person[i],0);
        if(j>max){
            max=j;
            agepath=&person[i];
        }
    }
    writebyage(agepath);
    return 0;
}

node_t *readF(char *fn,int *nppl){
    FILE *fp;
    node_t *person;
    node_t *currch,*currdad,*currmum;
    int n,i;
    char sex;
    char childN[MAX],childL[MAX];
    char dadN[MAX],dadL[MAX],mumN[MAX],mumL[MAX];
    fp=fopen(fn,"r");
    if(fp==NULL){
        printf("error in file opening!\n");
        exit(0);
    }
    fscanf(fp,"%d",&n);
    person=(node_t *)malloc(n*sizeof(node_t));
    if(person==NULL){
        printf("error in file opening!\n");
        exit(0);
    }
    i=0;
    while(i<n){
        fscanf(fp,"%s %s %c %d",person[i].name,person[i].last,&sex,&person[i].age);
        person[i]=set2Null(person[i]);
        if(sex=='M'){
            person[i].sex=M;
        }else{
            person[i].sex=F;
        }
    i++;
    }
    while(fscanf(fp,"%s %s %s %s %s %s",childN,childL,dadN,dadL,mumN,mumL)!=EOF){
        currch=search(person,childN,childL,n);
        currdad=search(person,dadN,dadL,n);
        currmum=search(person,mumN,mumL,n);
        currch=assignparent(currmum,currdad,currch);
        currmum=assignchild(currmum,currch);
        currdad=assignchild(currdad,currch);
    }
    fclose(fp);
    *nppl=n;
    return person;
}
node_t set2Null(node_t person){
    person.children=person.father=person.mother=person.next=NULL;
    person.nextage=person.nextsex=NULL;
    return person;
}
node_t *search(node_t *person,char *name,char *last,int n){
    int i;
    for(i=0;i<n;i++){
        if(strcmp(person[i].name,name)==0 && strcmp(person[i].last,last)==0){
            return &person[i];
        }
    }
    return NULL;
}
node_t *assignparent(node_t *mum,node_t *dad, node_t *child){
    child->mother=mum;
    child->father=dad;
    return child;
}
node_t *assignchild(node_t*parent,node_t *child){
    child->next=parent->children;
    parent->children=child;
    return parent;
}
int pathsex(node_t *person,sex sex,int cnt){
    node_t *ptr,*tmp;
    int i;
    ptr=person;
    tmp=person->children;
    if(ptr==NULL || ptr->sex!=sex){
        return cnt;
    }
    cnt++;
    while(ptr->children!=NULL){
        i=pathsex(ptr->children,sex,cnt);
        if(i>cnt){
            cnt=i;
            (*person).nextsex=ptr->children;
        }
        ptr->children=ptr->children->next;
    }
    ptr->children=tmp;
    return cnt;
}
void writebysex(node_t *ptr){
    while(ptr!=NULL){
        printf("%s %s\n",ptr->name,ptr->last);
        ptr=ptr->nextsex;
    }
    return;
}
int pathage(node_t *person,int age){
    node_t *ptr,*tmp;
    int i,max=0;
    ptr=person;
    tmp=person->children;
    if(ptr==NULL)
        return age;

    if(ptr->children==NULL)
        return age+person->age;
    while(ptr->children!=NULL){
        i=pathage(ptr->children,age+ptr->age);
        if(i>max){
            max=i;
            person->nextage=ptr->children;
        }
        ptr->children=ptr->children->next;
    }
    ptr->children=tmp;
    return max;
}
void writebyage(node_t *ptr){
    while(ptr!=NULL){
        printf("%s %s\n",ptr->name,ptr->last);
        ptr=ptr->nextage;
    }
    return;
}
