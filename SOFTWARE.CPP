#include<iostream.h>
#include<string.h>
#include<conio.h>
#include<fstream.h>
#include<math.h>
#include<ctype.h>
#include<graphics.h>
#include<time.h>
#include"img.h"
//img.h required for getimg()
float prec = 20;		 //Precision
float scale=50;			 //Not greater than 100
void pause(){			 // system("pause") for graphics
	cout<<"Press any key to continue...";
	getch();
}
void clr(){ 			//Clearing screen in Graphics
	setbkcolor(0);
	cleardevice();
	gotoxy(1,1);
}
int height(){          		//Text mode screen size
	int i=1;
	int x = wherex(), y = wherey(); //Save current cursor position
	while(wherey()!=(i-1)){
		gotoxy(1,++i);
	}
	gotoxy(x,y);		//Return back to original position
	return (i-1);
}
int width(){          		//Text mode screen size
	int i=1;
	int x = wherex(), y = wherey(); //Save current cursor position
	gotoxy(1,1);
	while(wherex()!=(i-1)){
		gotoxy(++i,1);
	}
	gotoxy(x,y);		//Return back to original position
	return (i-1);
}

int maxx;
int maxy;
		//For graph
void Line(float slope, float intercept){
	float x,y;
	for (x=-maxx/2;x<maxx;x++){
		y=slope*x+intercept;
		putpixel(maxx/2+x,maxy/2-y,9);
		}
	}
void Line(int x1,int y1, int x2, int y2)
	{
	line(maxx/2+x1,maxy/2-y1,maxx/2+x2,maxy/2-y2);
	}
void Axes(){
	line(maxx/2,0,maxx/2,maxy);
	line(0,maxy/2,maxx,maxy/2);
	}
void mark(){
	int x,y,color;
	color=WHITE;
	setcolor(8);
	for (x=scale;x/scale<=maxx/2;x+=scale){
		Line(x,maxy/2,x,-maxy/2);
		Line(-x,maxy/2,-x,-maxy/2);
	}
	for (y=scale;y/scale<=maxy/2;y+=scale){
		Line(maxx/2,y,-maxx/2,y);
		Line(maxx/2,-y,-maxx/2,-y);
	}
	setcolor(color);
	for (x=scale;x/scale<=maxx/2;x+=scale){
		Line(x,2,x,-2);
		Line(-x,2,-x,-2);
	}
	for (y=scale;y/scale<=maxy/2;y+=scale){
		Line(1,y,-1,y);
		Line(1,-y,-1,-y);
	}
}
int size = 0; //Size of the stack;
int top = -1;
fstream fl("FL.TXT",ios::out|ios::in|ios::trunc);
char *infix, *postfix, *Stack;
double *eval;
		//Calculation
/*--------Function prototypes-------*/
void getinput();
int precedence(char ch);	   //Function to get precedence of operator
char Pop();			   //Function to pop an element
char Topelement();		   //Function to return top element without popping
void Push(char ch);		   //Function to push an element in the stack
int braces(char *s);		   //Function to match number of braces
void intopost();
int posteval(double x);
void sendfeedback();
int showfeedback();


/*----End of Function Prototypes----*/

/*--------Function Definitions-------*/
void getinput(){
	char ch = 0;
	size=0;
	while(ch!='\r'){
		fl.seekp(size, ios::beg);
		ch = getch();
		if(ch==' ')
			continue;
		else if(ch=='\b'){
			if(size)
				size--;
			cout<<"\b \b";
		}
		else{
			fl<<ch;
			cout<<ch;
			size++;
		}
	}
	cout<<'\n';
	infix = new char[2 * size];
	int i = 0;
	fl.seekg(0, ios::beg);
	while(i < size-1){
		fl>>ch;
		infix[i++] = ch;
	}
	infix[i] = '\0';
	postfix = new char [2 * size];
	Stack = new char [2 * size];
}
int precedence(char ch){	  //Function to get precedence of operator and/or the math functions
	switch(ch){
		case 'm': return 12;		//fabs()
		case 'f': return 11;		//floor()
		case 'n': return 10;		//ln()
		case 'l': return 9;		//log()
		case 't': return 8;		//tan()
		case 's': return 7;		//sin()
		case 'c': return 6;             //cos()
		case '^': return 5;
		case '/': return 4;
		case '*': return 3;
		case '-': return 2;
		case '+': return 1;
		default : return 0;
	}
}
char Pop(){			  //Function to pop an element
	char ret;
	if(top!=-1){
		ret = Stack[top--];
		return ret;
	}
	else
		return '#';
}
char Topelement(){		  //Function to return top element without popping
	char ch;
	if(top!=-1)
		ch = Stack[top];
	else
		ch = '#';
	return ch;
}
void Push(char ch){		  //Function to push an element in the stack
	if (top!=size-1)
		Stack[++top] = ch;
}
int braces(char *s){		  //Function to match number of braces
	int leftbr,rightbr;
	leftbr = rightbr = 0;
	for (int i = 0; s[i]; i++){
		if (s[i]=='(')
			leftbr++;
		else if(s[i]==')')
			rightbr++;
	}
	if(leftbr==rightbr)
		return 0;
	else if(leftbr<rightbr)
		return 1;
	else return -1;
}
void intopost(){
	char ele,elem,st[2];
	int prep,pre,popped,j=0,chk=1;
	strcpy(postfix, " ");
	while(chk!=0){
		getinput();
		chk = braces(infix);
		if(chk!=0){
			cout<<"Unbalanced no. of braces.\nExtra ";
			cout<<(chk==1?"right braces":"left braces")<<'\n';
		}
	}
	for (int i=0;infix[i]!='\0';i++){
		//cout<<i;   <--For debugging
		if  (precedence(infix[i])==0 && infix[i]!='(' && infix[i]!=')')
			postfix[j++]=infix[i];
		else if(infix[i]=='('){
			elem = infix[i];
			Push(elem);
		}
		else if(infix[i]==')'){
			while((popped = Pop())!='(')
				postfix[j++] = popped;
		}
		else {
			elem = infix[i];
			pre = precedence(elem);	//Precedence of operator from infix
			ele = Topelement();
			prep = precedence(ele); //Precedence of operator from top of stack
			if(pre > prep)
				Push(elem);
			else{
				while(prep >= pre){
					if(ele=='#')
						break;
					popped = Pop();
					ele = Topelement();
					postfix[j++] = popped;
					postfix[j++] = ' ';
					prep = precedence(ele);
				}
				Push(elem);
			}
		}
		if  (!(precedence(infix[i+1])==0 && infix[i+1]!='(' && infix[i+1]!=')'))
			postfix[j++] = ' ';
	}
	while((popped = Pop()) != '#'){
		postfix[j++] = ' ';
		postfix[j++] = popped;
	}
	postfix[j] = '\0';
}
int posteval(double x){
	eval = new double(strlen(postfix));
	double num=0,result=0;
	int v=0,dec=0,top=-1,i=0;
	char ch;
	int success=1;
	while(postfix[i]!='\0' && i<strlen(postfix)){
		v=dec=0;
		num=0;
		ch = postfix[i];
		if (ch==' '){
			i++;
			continue;
		}
		if (isdigit(ch)){
			while(ch!=' ' && postfix[i]!=0){
				if(v)
					dec++;
				if(ch=='.')
					v=1;
				else
					num = (ch-'0') + num*10;
				ch = postfix[++i];
			}
			num/=pow(10,dec);
			eval[++top] = num;
			i++;
		}
		else if (ch=='x'||ch=='X'){
			eval[++top] = x;
			i++;
		}
		else if (ch=='e'||ch=='E'){
			eval[++top] = M_E;
			i++;
		}
		else if (ch=='p'||ch=='P'){
			eval[++top] = M_PI;
			i++;
		}
		else {
			result = 0;
			switch(ch){
				case 'm':
					result = fabs(eval[top]);
					top++;
					break;
				case 'f':
					result = floor(eval[top]);
					top++;
					break;
				case 'n':
					if(eval[top]>0){					//Error checking
						result = log(eval[top]);
						}
					else
						success = 0;
					top++;
					break;
				case 'l':
					if(eval[top]>0){
						result = log10(eval[top]);
						}
					else
						success = 0;
					top++;
					break;
				case 't':
					for (int i=0;(2*i+1)*M_PI_2<=scale*maxx;i++){
						if (fabs(eval[top])+(0.1/scale)>=(2*i+1)*M_PI_2&&fabs(eval[top])-(0.1/scale)<=(2*i+1)*M_PI_2)
							success = 0;
						else if (scale*fabs(tan(eval[top]))>=10e3)
							success = 0;
						else
							result = tan(eval[top]);
						}
					top++;
					break;
				case 's':
					result = sin(eval[top]);
					top++;
					break;
				case 'c':
					result = cos(eval[top]);
					top++;
					break;

				case '^':
					if (eval[top-1]==0){
						if (eval[top]<=0)
							success = 0;
						}
					else if (eval[top-1]<0 && ceil(eval[top])!=eval[top])
						success = 0;

					else
						result = pow(eval[top-1],eval[top]);
					break;
				case '/':
					if (eval[top] == 0)
						success = 0;
					else
						result = eval[top-1]/eval[top];
					if (scale*fabs(result)>=10e3)
						success = 0;
					break;
				case '*':
					result = eval[top-1]*eval[top];
					break;
				case '+':
					result = eval[top-1]+eval[top];
					break;
				case '-':
					result = eval[top-1]-eval[top];
					break;
			}
			if (scale*fabs(result)>=10e3)
				success = 0;
			eval[--top] = result;
			++i;
		}
	}
	return success;
}
class Feed{
	char name[30];
	char email[30];
	char feed[500]; //User's message
	time_t t;
	public:
	Feed(){
	t = time(0);
	}
	void getd(){
		cout<<"\n Enter Name  : ";
		gets(name);
		cout<<" Enter Email : ";
		gets(email);
		cout<<" Enter Message (max 500 chars) :\n";
		gets(feed);
	}
	void putd(){
		int wt=width();
		cout<<"\n Name  : "<<name;
		gotoxy(wt-30,wherey());
		cout<<"At : ";
		cout<<ctime(&t);
		cout<<" Email : "<<email;
		cout<<"\n Message : "<<feed;
	}
};

int showfeedback(){
	clr();
	Feed post;
	int i=1;
	int n=4;	      //Max entries per page
	fstream rev("FEED.DAT",ios::in|ios::binary);
	if (!rev)
		return 0;
	clr();
	while(!rev.eof()){
		rev.read((char*)&post,sizeof(post));
		if(rev.eof()){
			if(i%n!=1){
			cout<<"\n\n";
			pause();
			}
			break;
		}
		post.putd();
		cout<<"\n\n";
		if(i%n==0){
			pause();
			clr();
		}
		i++;
		}
	rev.close();
	return 1;
}
void sendfeedback(){

	Feed post;
	post.getd();
	post.putd();
	getch();
	fstream rev("FEED.DAT",ios::out|ios::app|ios::binary);
	rev.write((char*)&post,sizeof(post));
	rev.close();
}

/*----End of Function Definitions----*/

void main(){
clrscr();
int choice=1;
int ht = height();
while(choice){
getimg();
cout<<"\t\t\t\t    Menu\n";
cout<<" 1. Plot a function.\n";
cout<<" 2. Help and Documentation.\n";
cout<<" 3. View Submitted feedback.\n";
cout<<" 4. Send feedback/Request a feature.\n";
cout<<" 0. Exit\n\n";
cout<<" Enter your choice : ";
cin>>choice;
switch(choice){
case 1:
	cout<<" Enter the function below :\n> ";
	intopost();
	cout<<" Enter the scale for the graph (between 5 and 100) : ";
	cin>>scale;
	if (scale>100 || scale <5)
		scale = 50;//Default
	cout<<" Enter the precision (1 to 50) :";
	cout<<"\n [Warning : tan() will be slow with high precision, unless the CPU is fast]\n> ";
	cin>>prec;
	if(prec<1||prec>50)
		prec = 20; //Default
	pause();
	int gd=DETECT,gm;
	initgraph(&gd, &gm, "c:/tc/bgi ");
	maxx = getmaxx();
	maxy = getmaxy();
	double x, y;
	mark();
	Axes();
	for (x=(-maxx/2)/scale;x<maxx;x+=(1.0/((scale)*prec))){
		if(!posteval(x))
			continue;
		y=scale*(eval[0]);
		delete eval;
		if (x*scale>=maxx/2)
			break;
		x=x*scale;
		if (y>(maxy)*scale||y<(-maxy)*scale)
			continue;
		else
			putpixel(maxx/2+x,maxy/2-y,LIGHTGREEN);
		x=x/scale;
	//	delay(0);
	}
	getch();
	break; //End of choice 1
case 2:
	clr();
	fstream doc("doc.txt", ios::in);
	if(!doc){
		cout<<" Documentation unavailable!";
		getch();
	}
	else{
		char ch;
		while(!fl.eof()){
			ch = doc.get();
			if(doc.eof()){
				cout<<"\n";
				pause();
				break;
				}
			if(ch=='\n' && wherey()==(ht-2)){
				cout<<ch;
				pause();
				clr();
			}
			else{
				if(wherey()==(ht-2)){
					cout<<'\n';
					pause();
					clr();
				}
				cout<<ch;
			}
		}
	}
	doc.close();
	break;
case 3:
	clr();
	if(!showfeedback()){
		cout<<" No feedback history available!";
		getch();
	}
	break;
case 4:
	sendfeedback();
	getch();
	break;
}
closegraph();
restorecrtmode();
}
closegraph();
restorecrtmode();
fl.close();
}
