#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#define HASH_SIZE 100
#define URL_LENGTH 1000
#define MAX_URL_PER_PAGE 15
#define MAX_DEPTH 3
#define SEED_URL  "https://www.chitkara.edu.in"
#define INTERVAL_TIME 10
#define CHECK_MALLOC(s) if ((s) == NULL) { \
printf("memory allocation failed in %s: line%d", _FILE, __LINE_); \
perror(":"); \
exit(-1); \
}
#define SET_MEM(start,size) memset(start,0,size)
static int fno=1,currdepth=1;
struct urltype
{
	char pagelink[URL_LENGTH];
	int depth;
};
struct node
{
  	struct urltype u;
	 struct node*next;
	  int visited;
          int key; 
};
struct hash
{
  struct node*clusthead;
  struct node*clusttail;
  int count;
};
struct hash h[HASH_SIZE];
struct node*head=NULL;
struct node*create(char *s1,int dep,int v)
{
	struct node*temp=(struct node*)malloc(sizeof(struct node));
	strcpy(temp->u.pagelink,s1);
	temp->u.depth=dep;
         temp->visited=v;
	temp->next=NULL;
	return temp;
}
void printlinkedlist()
{
printf("linked list is\n");
struct node*t;
  t=head;
     while(t!=NULL)
     {
         printf("%s  %d  %d  %d  %d\n",t->u.pagelink,t->u.depth,t->visited,t->key,h[t->key].count);
         t=t->next;
     }
exit(1);
}
 void display(char *dir)
 {
   struct node*t;
     if(head==NULL)
        printf("EMPTY LIST\n");
     else
       {
         t=head;
         while(t!=NULL)
         {
           if(t->u.depth<MAX_DEPTH && t->visited==0)
            {
             t->visited=1;
             currdepth=t->u.depth+1;
             fetchpage(t->u.pagelink,dir);
            }
            t=t->next;
           }
        }
       if(t==NULL)  
       printlinkedlist();
 }
int generatekey(char*s3)
{
 int i=0,c=0;
   while(s3[i]!='\0')
   {
       c = c+((int)s3[i]/2);
       i++;
  }
    while(c>100)
    c = c/50;
      return c;
}
void tail_insertion(char strpointer[][URL_LENGTH],int n,char*dir)
{
    struct node*t,*temp;
    struct hash*temp1,*t1;
	int i=0,k=0;
	for(i=0;i<n;i++)
	{
		int flag1=0,flag2=0;
		temp=create(strpointer[i],currdepth,0);
		k=generatekey(strpointer[i]);
                 if(h[k].clusthead==NULL)
                 { 
                  h[k].clusthead=temp;
                  h[k].clusttail=temp;
				  temp->key=k;
				   t=head;
				  while(t->next!=NULL)
				  t=t->next;
				  t->next=temp;
				  temp->next=NULL;
				  h[k].clusttail=temp; 
				  h[k].count++;  
                 }
                else
                {
                 t=h[k].clusthead;
				 while(t!=h[k].clusttail)
			     {
			     	 if(strcmp(t->u.pagelink,temp->u.pagelink)==0)
			     	  {
			     	  	flag1=1;
			     	  	break;
					   }
			     	 t=t->next;
				 }
				 if(flag1==0){
				 if(t->next!=NULL)
				 {
				 	//midinsertion
				 	struct node*new;
				 	new=t->next;
				    temp->key=k;
				   t->next=temp;
				  temp->next=NULL;
				  h[k].clusttail=temp; 
				  h[k].count++;	
				  t=t->next;
				  t->next=new;
				  }
				 else
				 {
				    temp->key=k;
				   t->next=temp;
				  temp->next=NULL;
				  h[k].clusttail=temp; 
				  h[k].count++;	
				 }
				  	
				}
			}

               
	}
	display(dir);
      //  printlinkedlist();
}
void inserthead(char*s1)
{
 struct node*temp;
 temp=create(s1,0,1);
int k=generatekey(s1);
                 if(h[k].clusthead==NULL)
                 { 
                  h[k].clusthead=temp;
                  h[k].clusttail=temp;  
                 }
 temp->key=k;
 h[k].count++;
if(head==NULL)
head=temp;   
}
int validatelink(char*s1,char*url)
{
  char matchlink[100];
  strcpy(matchlink,SEED_URL);
  int i=0,val=1,flag=0;
  if(strlen(s1)<=strlen(matchlink)+2)
  val=0;
  if(strcmp(s1,url)==0)
   val=0;
 while(matchlink[i]!='\0')
 {
   if(matchlink[i]!=s1[i])
    {
     flag=1;
     break;
    }
    i++;
 }
 if(flag==1)
 val=0;
 return val; 
}
void fetchlinks(char*str,char*url,char*dir)
{
//printf("***************************************\n");
int urlcount=0;
int i=0,j=0,k=0,f=0,f1=0;
char s[URL_LENGTH +30],s1[URL_LENGTH +30];
i=0;
   char strpointer[MAX_URL_PER_PAGE][URL_LENGTH];
  while(str[i]!='\0')
  {
  	j=0;
  	f=0;
  	if(str[i]=='<'&& str[i+1]=='a')
  	{
  		f=1;
  		while(str[i]!='>')
  		{
  			s[j]=str[i];
  			 j++;
  			 i++;
		}
		s[j]='\0';  
	}
	if(f==1)
	{ 
		j=0;
		k=0;
		f1=0;
		f=0;
		while(s[j]!='\0')
		{
			
			if(s[j]=='h'&&s[j+1]=='r'&&s[j+2]=='e')
			{
				while(s[j]!='"')
				j++;
				 while(s[j+1]!='"')
				 {
				 	s1[k]=s[j+1];
				 	k++;
				 	j++;
				 }
				 s1[k]='\0';
				 if(strlen(s1)>=5)
				 {
			        //printf(" %s\n",s1); 
                              f1=validatelink(s1,url);
                            //  printf("%d\n",f1);  
                              if(f1==1)
                              {
                            int p=0,fl=0;
                           if(urlcount< MAX_URL_PER_PAGE )	
                           {
                        	for(p=0;p<urlcount;p++)
  	                        {
                            	if(strcmp(strpointer[p],s1)==0)	
                                  {	 fl=1;
                                    break;
                                  }
                           	}
                           	if(fl==0)
							{
  	                        strcpy(strpointer[urlcount],s1);
                           	//printf("url %s\n",strpointer[urlcount]);
                             	urlcount++;
                             }
                       }
                                 
                              }    
			    }
			}
			else
			j++;
		}
	}
	else
	i++;
  }
int x = urlcount;
tail_insertion(strpointer,x,dir);
free(str);
}
void createfile(char*url,char*dir)
{
  char dirname[100];
  char newfile[100],newdirname[100];
  strcat(dirname,dir);
  strcat(dirname,"/temp.txt");
  FILE *fp1,*fp2;
  fp1 = fopen(dirname,"r");
 // fseek(fp1,0L,SEEK_END);
  //long int size = (ftell(fp1)+1);  
 struct stat st; //variable which will count length of file.
stat(dirname,&st); // temp.txt is the file where wget fetch the html
int size=st.st_size;
char *ptr=(char*)malloc(size*sizeof(char));
  sprintf(newfile,"%d",fno);
  fno++;
  strcat(newfile,".txt");
  strcat(newdirname,dir);
  strcat(newdirname,"/");
  strcat(newdirname,newfile);
 // rewind(fp1);
  fp2 = fopen(newdirname,"w");
  fputs(url,fp2);
  fputs("\n",fp2);
  char p[10];
  sprintf(p,"%d",currdepth-1); 
  fputs(p,fp2);
  fputs("\n\n",fp2);
  int i=0;
  char ch;
  ch=fgetc(fp1);
  while(ch!=EOF)
  {
  	ptr[i]=ch;
  	i++;
  	fputc(ch,fp2);
  	ch=fgetc(fp1);
  }
  ptr[i]='\0';
 fclose(fp1);
 fclose(fp2);
 fetchlinks(ptr,url,dir);
}

void fetchpage(char*url,char*dir)
{
	char urlbuffer[URL_LENGTH+300]={0};
	strcat(urlbuffer,"wget --no-check-certificate -O ");
	strcat(urlbuffer,dir);
	strcat(urlbuffer,"/temp.txt ");
	strcat(urlbuffer,url );
	system(urlbuffer);
        createfile(url,dir);    
}
int checkdepth(char *str)
{ 
 int i=0,dept=0;
  while(str[i]!='\0')  
  {
    int p=0;
    p = str[i]-'0';
   if(!(p>=0&&p<=9))
    {	
	return 0;
	}
  dept=dept*10+p;
  i++;
  }
  if(dept>MAX_DEPTH)
   {
   printf("DEPTH OUT OF RANGE\n");
   return 0;
   }
   else
   {
    printf("valid depth\n");
   return 1;
 }
}
int checkdir(char *dir)
{
  struct stat statbuf;
  if ( stat(dir, &statbuf) == -1 ) 
  {
    printf("Invalid directory\n");
    return 0;
  }

  //Both check if there's a directory and if it's writable
  if ( !S_ISDIR(statbuf.st_mode) ) 
  {
    printf("Invalid directory entry. Your input isn't a directory\n");
    return 0;
  }

  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR ) 
  {
    printf("Invalid directory entry. It isn't writable\n");
    return 0; 
  }
  printf("Valid directory\n");
  return 1;
}
int checklink(char*str,char*dir)
{
  if(strlen(str)>URL_LENGTH)
   { 
   printf("url too long\n");
   return 0;
   }
char link[URL_LENGTH+50]="wget --no-check-certificate --spider ";
strcat(link,str);
if(!system(link))
{
 printf("Valid URL\n");
 inserthead(str);
 fetchpage(str,dir); 
}
else
{
printf("Invalid URL\n");
return 0;	
}
}

int main(int argc,char*argv[])
{
	int vdepth=1,vdir=1,vlink=1;
  if(argc>4)
   exit(1);
    vdepth = checkdepth(argv[2]);
    if(!vdepth)
     exit(1);
   vdir = checkdir(argv[3]);
      if(!vdir)
        exit(1);
        vlink = checklink(argv[1],argv[3]);
        if(!vlink)
        exit(1);
}
