/*Environment: 
*1Visual C++6.0,VisualC++.NET and Visual C++.NET 2003
*WinNT, Win 200, WinXP and  Win2003
*2 gcc version 3.3.2  20031022(Red Hat Linux 3.3.2-1)
*Redhat Fedora core 1
*Intel 32 platform
*
*How to use:
*Enter the file name of your PL/0 source code
*answer if you want to print the object code
*answer if you want to print the symbol table
*
*find your files below:
*fa.tmp 			P-code virtual machine code
*fa1.tmp  		source file and the address that its each line corresponds to
*fa2.tmp  		result
*fas.tmp  		symbol table
*fa3_data.s 	MIPS code
*/
#include<stdio.h>
#include"pl0.h"
#include"string.h"
/*Stack for the interpreter*/
#define stacksize 500


/*
* Files below are for outputing MIPS assembly language
*/
FILE* fa3;
FILE* fa3_data;
int fa3_size;

int main()
{
	int i = 0;
	bool nxtlev[symnum];
	printf("Input pl/0 file ?");
	scanf("%s", fname);                                     /*input file name*/
	fin = fopen(fname, "r");

  /*
  * write to fa3.s, which is for MIPS code
  */
  fa3 = fopen("fa3.s", "w");

  fa3_data = fopen("fa3_data.s", "w");
  fprintf(fa3, "\t.text\n\t.globl main\n");
  labelnum = 0;

	if (fin)
	{
		printf("List object code ?(Y/N)");                /*output object code?*/
		scanf("%s", fname);
		listswitch = (fname[0] == 'y' || fname[0] == 'Y');
		printf("List symbol table ? (Y/N)");             /*output symbol table?*/
		scanf("%s", fname);
		tableswitch = (fname[0] == 'y' || fname[0] == 'Y');
		fa1 = fopen("fa1.tmp", "w");
		fprintf(fa1, "Iput pl/0 file ?");
		fprintf(fa1, "%s\n", fname);
		init();                                          /*initialize*/
		err = 0;
		cc = cx = ll = 0;
		ch = ' ';
		if (-1 != getsym())
		{
			fa = fopen("fa.tmp", "w");
			fas = fopen("fas.tmp", "w");
			addset(nxtlev, declbegsys, statbegsys, symnum);
			nxtlev[period] = true;
			char mainproc[al + 1] = "main";
			if (-1 == block(0, 0, nxtlev, mainproc))			/*call compiling function*/
			{
				fclose(fa);
				fclose(fa1);
				fclose(fas);
				fclose(fin);

				printf("\n");
				return 0;
			}
			fclose(fa3);
			fclose(fa3_data);
			// connect the two files fa3 and fa3_data
      FILE * fp = fopen("fa3.s", "r");
      fseek(fp, 0L, SEEK_END);
      fa3_size = ftell(fp);
      fclose(fp);

      fa3 = fopen("fa3.s", "rb");
      if (fa3 == NULL) {
          printf( "Source file open failure.");
          return -1;
      }
      fa3_data = fopen("fa3_data.s", "ab");		// open in append mode
      if (fa3_data == NULL) {
          printf("Destination file open failure.");
          return -1;
      }

      char buffer[1024]={0};
      i = 1;

      while ((i * 1024) < fa3_size) {
          fread(buffer,1,1024,fa3);
          fwrite(buffer,1,1024,fa3_data);
          i++;
      }
      fread(buffer, 1, (fa3_size - (i - 1) * 1024), fa3);
      fwrite(buffer, 1, (fa3_size - (i - 1) * 1024), fa3_data);
      fclose(fa3);
      fclose(fa3_data);
			fclose(fa);
			fclose(fa1);
			fclose(fas);
			if(sym != period)
			{
				error(9);
			}
			if(err == 0)
			{
				fa2 = fopen("fa2.tmp", "w");

				interpret();
				fclose(fa2);
			}
			else
			{
				printf("Errors in pl/0 program");
			}
		}
		fclose(fin);
	}
	else
	{
		printf("Can't open file! \n");
	}
	printf("\n");
	return 0;
}

/*
* initialize
*/
void init()
{
	int i;
	for(i=0;i<=255;i++)
	{
		ssym[i]=nul;
	}
	ssym['+']=plus;
	ssym['-']=minus;
	ssym['*']=times;
	ssym['/']=slash;
	ssym['(']=lparen;
	ssym[')']=rparen;
	ssym['=']=eql;
	ssym[',']=comma;
	ssym['.']=period;
	ssym['#']=neq;
	ssym[';']=semicolon;
	/*Set reserved word names. Keep them in alphabetical order because we use binary search*/
	strcpy(&(word[0][0]),"begin");
	strcpy(&(word[1][0]),"call");
	strcpy(&(word[2][0]),"const");
	strcpy(&(word[3][0]),"do");
	strcpy(&(word[4][0]),"end");
	strcpy(&(word[5][0]),"if");
	strcpy(&(word[6][0]),"odd");
	strcpy(&(word[7][0]),"procedure");
	strcpy(&(word[8][0]),"read");
	strcpy(&(word[9][0]),"then");
	strcpy(&(word[10][0]),"var");
	strcpy(&(word[11][0]),"while");
	strcpy(&(word[12][0]),"write");
	/*Set symbols for reserved words*/
	wsym[0]=beginsym;
	wsym[1]=callsym;
	wsym[2]=constsym;
	wsym[3]=dosym;
	wsym[4]=endsym;
	wsym[5]=ifsym;
	wsym[6]=oddsym;
	wsym[7]=procsym;
	wsym[8]=readsym;
	wsym[9]=thensym;
	wsym[10]=varsym;
	wsym[11]=whilesym;
	wsym[12]=writesym;

	/*Set names for MIPS instructions*/
	strcpy(&(mnemonic[lit][0]),"lit");
	strcpy(&(mnemonic[opr][0]),"opr");
	strcpy(&(mnemonic[lod][0]),"lod");
	strcpy(&(mnemonic[sto][0]),"sto");
	strcpy(&(mnemonic[cal][0]),"cal");
	strcpy(&(mnemonic[inte][0]),"int");
	strcpy(&(mnemonic[jmp][0]),"jmp");
	strcpy(&(mnemonic[jpc][0]),"jpc");

	/*Symbol sets*/
	for(i=0;i<symnum;i++)
	{
		declbegsys[i]=false;
		statbegsys[i]=false;
		facbegsys[i]=false;
	}

	/*Symbol sets for beginning of a declaration*/
	declbegsys[constsym]=true;
	declbegsys[varsym]=true;
	declbegsys[procsym]=true;
	/*Symbol sets for beginning of a statement*/
	statbegsys[beginsym]=true;
	statbegsys[callsym]=true;
	statbegsys[ifsym]=true;
	statbegsys[whilesym]=true;
	/*Symbol sets for beginning of a factor*/
	facbegsys[ident]=true;
	facbegsys[number]=true;
	facbegsys[lparen]=true;
}
 /*
  * Set operations
  */
int inset(int e,bool* s)
{
    return s[e];
}
int addset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]||s2[i];
    }
    return 0;
}
int subset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&(!s2[i]);
    }
    return 0;
}
int mulset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&s2[i];
    }
    return 0;
}
/*
 * Exception handling. Print where an error happens and the error code 
 */
void error(int n)
{
	char space[81];
	memset(space,32,81); printf("-------%c\n",ch);
	space[cc-1]=0; // Symbols have all been read when error arises so we use cc-1
	printf("****%s!%d\n",space,n);
	err++;
}
/*
 *  Read one char (ignore spaces)
 *
 *  Read one line each time into line buffer. Get another line after getsym() has done this line.
 *
 *  Called by getsym()
 */
int getch()
{
	if(cc==ll)
    {
		if(feof(fin))
		{
			printf("program incomplete");
			return -1;
		}
		ll=0;
		cc=0;
		printf("%d ",cx );
		fprintf(fa1,"%d ",cx);
		ch=' ';
		while(ch!=10)
		{
      if(EOF==fscanf(fin,"%c",&ch))
			{
				line[ll]=0;
				break;
			}
			printf("%c",ch);
			fprintf(fa1,"%c",ch);
			line[ll]=ch;
			ll++;
		}
		printf("\n");
		fprintf(fa1,"\n");
	}
	ch=line[cc];
	cc++;
	return 0;
}
/*
 * Lexical analysis. Read one symbol (from a line).
 */

int getsym()
{
	int i,j,k;
	while( ch==' '||ch==10||ch==9)
	{
		getchdo;
	}
	if(ch>='a'&&ch<='z')
	{
		k=0;
		do{
			if(k<al)
			{
				a[k]=ch;
				k++;
			}
			getchdo;
		}while(ch>='a'&&ch<='z'||ch>='0'&&ch<='9');
		a[k]=0;
		strcpy(id,a);
		i=0;
		j=norw-1;
		do{
			k=(i+j)/2;
			if(strcmp(id,word[k])<=0)
			{
				j=k-1;
			}
			if(strcmp(id,word[k])>=0)
			{
				i=k+1;
			}

		}while(i<=j);
		if(i-1>j)
		{
			sym=wsym[k];
		}
		else
		{
			sym=ident;
		}
	}
	else
	{
		if(ch>='0'&&ch<='9')
		{
			k=0;
			num=0;
			sym=number;
			do{
				num=10*num+ch-'0';
				k++;
				getchdo;
			}while(ch>='0'&&ch<='9'); /*Parse a number*/
			k--;
			if(k>nmax)
			{
				error(30);
			}
		}
		else
		{
			if(ch==':')             /*Check for an ':=' */
			{
				getchdo;
				if(ch=='=')
				{
					sym=becomes;
					getchdo;
				}
				else
				{
					sym=nul;            /*unrecognizable*/
				}
			}
			else
			{
				if(ch=='<')         /*Check if it's a '<' or '<='*/
				{
					getchdo;
					if(ch=='=')
					{
						sym=leq;
						getchdo;
					}
					else
					{
						sym=lss;
					}
				}
				else
				{
					if(ch=='>')          /*Check if it's a '>' or '>='*/
					{
						getchdo;
						if(ch=='=')
						{
							sym=geq;
							getchdo;
						}
						else
						{
						    sym=gtr;
						}
					}
					else
					{
						sym=ssym[ch];	/*If none of the symbols above is find, regard it as a simgle word*/
						//richard
						if(sym!=period)
						{
							getchdo;
						}
						//end richard
					}
				}
			}
		}
	}
	return 0;
}
/*
* Generate virtual machine code
*
*x:instruction.f;
*y:instruction.l;
*z:instruction.a;
*/
int gen(enum fct x,int y,int z)
{
	if(cx>=cxmax)
	{
		printf("Program too long"); /*Too long a program*/
		return -1;
	}
	code[cx].f=x;
	code[cx].l=y;
	code[cx].a=z;
	cx++;
	return 0;
}
/*
* Is this next symbol legal?
*
* At the end of some specific part (e.g. a statement, or an expression)
*  we need the next symbol to belong to some certain set (s1), FOLLOW SET.
* This function test() does this.
* If the symbol fails the test, we furthermore have another set (s2) to test;
*  it is acceptable if the symbol belongs to it, and the compile can ignore the 
*  first error and carry on compiling the rest of the file.
* If the symbol fails any test, provide an error code.
*
*S1£ºThe set that we wish this symbol belongs to
*s2: If it doesn't, then s2 is the set we need to furthermore test
*n: Error number
*/
int test(bool* s1,bool* s2,int n)
{
  if(! inset(sym,s1))
  {
		error(n);
		/* Will not stop if it tests an error. */
		while((! inset(sym,s1))&&(! inset(sym,s2)))
		{
			getsymdo;
		}
  }
  return 0;
}
/*
 * Compile function
 *
 *lev: the 'level' that the current subprogram is in
 *tx: tail of the symbol table
 *fsys: the follow set
 */
int block(int lev,int tx,bool* fsys, char *procname)
{
    int i;
    int dx;                         /*relative address for a symbol*/
    int tx0;                        /*store the initial value of tx*/
    int cx0;                        /*store the initial value of cx*/
    bool nxtlev[symnum];            /*Create some space for the symbol set from the next level.
																			So that the set of this level won't be changed*/
    dx=3;
    tx0=tx;                         /*initial position of the current level's names*/
    table[tx].adr=cx;
    gendo(jmp,0,0);
    if(lev > levmax)
    {
		error(32);
    }

    if (lev == 0){
      // generate a .data file. But only do this in the main program (level is 0)
      fprintf(fa3_data,"\t.data\n");
    }

    do{
      if(sym==constsym) /*If a const declaration symbol is found*/
      {
				getsymdo;
				do{
					constdeclarationdo(&tx,lev,&dx);   /* use pointers because dx will be changed by constdeclaration*/
					while(sym==comma)
					{
						getsymdo;
						constdeclarationdo(&tx,lev,&dx);
					}
					if(sym==semicolon)
					{
						getsymdo;
					}
					else
					{
						error(5); /*missing a ',' or a ';'*/
					}
				}while(sym==ident);
			}
			if(sym==varsym)	/*If a variable declaration symbol is found*/
			{
				getsymdo;
				do{
					vardeclarationdo(&tx,lev,&dx);
					while(sym==comma)
					{
						getsymdo;
						vardeclarationdo(&tx,lev,&dx);
					}
					if(sym==semicolon)
					{
						getsymdo;
					}
					else
					{
						error(5);
					}
				}while(sym==ident);
			}
			while(sym==procsym)/*If a procedure declaration symbol is found*/
			{
				// Record its name
				char this_proc_name[al + 1];

				getsymdo;
				if(sym==ident)
				{
					enter(procedur,&tx,lev,&dx);	/*enter the name into the symbol table*/
					strcpy(this_proc_name, id);
					getsymdo;
				}
				else
				{
					error(4);/*procedure should be followed by an identifier*/
				}
				if(sym==semicolon)
				{
					getsymdo;
				}
				else
				{
					error(5); /*a ';' is missing*/
				}
				memcpy(nxtlev,fsys,sizeof(bool)*symnum);
				nxtlev[semicolon]=true;

				if(-1==block(lev+1,tx,nxtlev, this_proc_name))
				{
					return -1;/*recursive calls*/
				}
				if(sym==semicolon)
				{
					getsymdo;
					memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
					nxtlev[ident]=true;
					nxtlev[procsym]=true;
					testdo(nxtlev,fsys,6);
				}
				else
				{
					error(5); /*missing a ';'*/
				}
			}
			memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
			nxtlev[ident]=true;
			nxtlev[period]=true;
			testdo(nxtlev,declbegsys,7);

    }while(inset(sym,declbegsys));                /*until no declaration is found*/
    code[table[tx0].adr].a=cx;                    /*generate code for this procedure*/
    table[tx0].adr=cx;                            /*the address for this code*/
		table[tx0].size = dx;													/*each declaration adds dx up by 1;
																										after declaration part, dx will be the
																										size of the data part of current procedure*/

		cx0=cx;
    gendo(inte,0,dx);                             /*generate memory allocation code*/

    // add everything in the symbol table into the .data file
    for (i = tx0 + 1; i <= tx; ++i){
      switch(table[i].kind)
      {
        case variable:
        fprintf(fa3_data,"var_%s:\t.word\t%d\n",table[i].name, 0);
        break;
			}
    }
    if(tableswitch)                               /*need to output symbol table?*/
    {
      printf("TABLE:\n");
      if(tx0+1>tx)
      {
				printf("NULL\n");
      }
      for(i=tx0+1;i<=tx;i++)
      {
        switch(table[i].kind)
        {
          case constant:
						printf("%d const %s  ",i,table[i].name);
						printf("val=%d\n",table[i].val);
						fprintf(fas,"%d const %s",i,table[i].name);
						fprintf(fas,"val=%d\n",table[i].val);
            break;
          case variable:
            printf("%d var  %s  ",i,table[i].name);
            printf("lev=%d addr=%d\n",table[i].level,table[i].adr);
            fprintf(fas,"%d var %s",i,table[i].name);
            fprintf(fas,"lev=%d addr=%d\n",table[i].level,table[i].adr);
            break;
          case procedur:
            printf("%d proc%s",i,table[i].name);
            printf("lev=%d addr=%d size=%d\n",table[i].level,table[i].adr,table[i].size);
						fprintf(fas,"%d proc%s",i,table[i].name);
						fprintf(fas,"lev=%d adr=%d size=%d \n",table[i].level,table[i].adr,table[i].size);
						break;
				}
			}
			printf("\n");
		}
		/*statement is followed by a ';' or 'end'*/
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);	/*add the previous level's follow set to current level's*/
		nxtlev[semicolon]=true;
		nxtlev[endsym]=true;

    // record the prodecure name in the file
    if (strcmp(procname, "main") == 0){
        fprintf(fa3, "main:\n");
    } else{
        fprintf(fa3, "proc_%s:\n", procname);
    }


		statementdo(nxtlev,&tx,lev);
		gendo(opr,0,0); /*add it in the end of every procedure!*/
		// end of a program

    fprintf(fa3, "\tjr $ra\n");
		memset(nxtlev,0,sizeof(bool)*symnum); /*If it's a subprogram, then no extra things is added to its follow set*/
		test(fsys,nxtlev,8);                  /*test the following symbols*/
		listcode(cx0);                        /*output code*/
		return 0;
}
/*
*Add to the symbol table
*
*k: what type?  const,var or procedure
*ptx: pointer to the tail of the table
*lev: what level the symbol is in
*pdx: relative addres for variables
*/
void enter (enum object k,int *ptx,int lev, int *pdx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name,id);       /*if it esists in the global ids?*/
	table[(*ptx)].kind=k;
	switch(k)
	{
		case constant:                      /*consts*/
			if (num>amax)
			{
				error(31);
				num=0;
			}
			table[(*ptx)].val=num;
			break;
		case variable:                     /*variables*/
			table[(*ptx)].level=lev;
			table[(*ptx)].adr=(*pdx);
			(*pdx)++;
			break;                          /*procedures*/
		case procedur:
			table[(*ptx)].level=lev;
			break;
	}

}
/*
 * Looking up in a symbol table
 * return the position (0 if not found)
 *
 * idt: symbol to look for
 * tx: tail pointer of current table
 */
int position(char *  idt,int  tx)
{
	int i;
	strcpy(table[0].name,idt);
	i=tx;
	while(strcmp(table[i].name,idt)!=0)
	{
		i--;
	}
	return i;
}
/*
 * const declaration
 */
int constdeclaration(int *  ptx,int lev,int *  pdx)
{
	if(sym==ident)
	{
		getsymdo;
		if(sym==eql ||sym==becomes)
		{
			if(sym==becomes)
			{
				error(1);                     /*mistake '=' for ':='*/
			}
			getsymdo;
			if(sym==number)
			{
				enter(constant,ptx,lev,pdx);
				getsymdo;
			}
			else
			{
				error(2);                  /*'=' should be followed by numbers*/
			}
		}
		else
		{
			error(3);                       /*identifier should be followed by '='*/
		}
	}
	else
	{
		error(4);                        /*"const" should be followed by indentifier*/
	}
	return 0;
}
/*
 *
 */
int vardeclaration(int * ptx,int lev,int * pdx)
{
	if(sym==ident)
 	{
 		enter(variable,ptx,lev,pdx);	// add to symbol table
 		getsymdo;
 	}
 	else
 	{
 		error(4);
    }
    return 0;
}
 /*
  * output/record opject code
  */
void listcode(int cx0)
{
	int i;
   	if (listswitch)
   	{
   		for(i=cx0;i<cx;i++)
   	 	{
   	 		printf("%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
   	 		fprintf(fa,"%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
   	    }
   	 }
}
/*
* parsing a statement
*/
int statement(bool* fsys,int * ptx,int lev)
{
	int i,cx1,cx2;
   	bool nxtlev[symnum];
   	if(sym==ident)
   	{
			i=position(id,*ptx);
   	 	if(i==0)
   	 	{
   	 		error(11);
   	 	}
   	 	else
   	 	{
   	 		if(table[i].kind!=variable)
   	 		{
   	 		 	error(12);
   	 		 	i=0;
   	 		}
   	 		else
   	 		{
   	 			getsymdo;
   	 			if(sym==becomes)
   	 			{
   	 				getsymdo;
   	 			}
   	 			else
   	 			{
   	 			 	error(13);
   	 			}
   	 			memcpy(nxtlev,fsys,sizeof(bool)* symnum);
   	 			expressiondo(nxtlev,ptx,lev);
   	 			if(i!=0)
   	 			{
   	 			 	gendo(sto,lev-table[i].level,table[i].adr);

   	 			 	// pop a number from a stack
            fprintf(fa3, "\tlw $t0, -4($sp)\n");
            // assign the number to a variable
            fprintf(fa3, "\tsw $t0, var_%s\n", table[i].name);
						// decrease stack's pointer
						fprintf(fa3, "\taddi $sp, $sp, -4\n");
   	 			}
   	 	  }
   		}
    }
    else
    {
    	if(sym==readsym)
    	{
    	 	getsymdo;
    	 	if(sym!=lparen)
    	 	{
    	 	   error(34);
				}
    	  else
				{
    	   	do{
    	   		getsymdo;
    	   	  if(sym==ident)
    	   	  {
    	   	  		i=position(id, *ptx);
    	   	  }
    	   	  else
    	   	  {
    	   	  		i=0;
    	   	  }
    	   	  if(i==0)
    	   	  {
    	   	  	error(35);
    	   	  }
    	   	  else
    	   	  {
    	   	   	gendo(opr,0,16);
							gendo(sto,lev-table[i].level,table[i].adr);	/* store to a variable*/

							// use system call to read I/O input
							fprintf(fa3, "\tli $v0, 5\n");
							fprintf(fa3, "\tsyscall\n");
							// save to a variable
							fprintf(fa3, "\tsw $v0, var_%s\n", table[i].name);
						}
						getsymdo;
					}while (sym==comma);	/*you can read multiple variables in a statement*/
				}
			if(sym!=rparen)
			{
				error(33);			/* should be a right parenthesis */
				while(!inset(sym,fsys)) /* try to continue compiling */
				{
					getsymdo;
				}
			}
			else
			{
				getsymdo;
			}
		}
		else
		{
			if(sym==writesym)			/* a write statemtnte. similar to the read statement*/
			{
				getsymdo;
				if(sym==lparen)
				{
					do{
						getsymdo;
						memcpy(nxtlev,fsys,sizeof(bool)*symnum);
						nxtlev[rparen]=true;
						nxtlev[comma]=true;		/* write can be followed by ')' or ','*/
						expressiondo(nxtlev,ptx,lev);/* calling an expression parsing function. different from a read!*/
						gendo(opr,0,14);/* generate an output instruction to output the number on the top of a stack*/

            // pop a number 
            fprintf(fa3, "\tlw $t0, -4($sp)\n");
            // into a register
            fprintf(fa3, "\tmove $a0, $t0\n");
            // decrease the pointer of the stack
            fprintf(fa3, "\taddi $sp, $sp, -4\n");
            // system call, output a number
            fprintf(fa3, "\tli $v0, 1\n");
						fprintf(fa3, "\tsyscall\n");
					}while(sym==comma);
					if(sym!=rparen)
					{
						error(33);/* incomplete expression*/
					}
					else
					{
						getsymdo;
					}
				}
				gendo(opr,0,15);		/* output an newline*/
			}
			else
			{
				if(sym==callsym)		/* call statement*/
				{
					getsymdo;
					if(sym!=ident)
					{
						error(14);           /*call should be followed by an identifier*/
					}
					else
					{
						i=position(id,*ptx);
						if(i==0)
						{
							error(11);          /*prodecure not found*/
						}
						else
						{
							if(table[i].kind==procedur)
							{
								gendo(cal,lev-table[i].level,table[i].adr);  /*call statement*/

                // push the number in $ra into stack
                fprintf(fa3, "\tsw $ra, ($sp)\n");
                // stack pointer ++
                fprintf(fa3, "\taddi $sp, $sp, 4\n");
								// call the function
								fprintf(fa3, "\tjal proc_%s\n", table[i].name);
								// a subprogram may need to pop
                // pop a number and store into $ra
                fprintf(fa3, "\tlw $ra, -4($sp)\n");
                // stack pointer --
                fprintf(fa3, "\taddi $sp, $sp, -4\n");
							}
							else
							{
								error(15);      /*call should be followed by an identifier that refers to a procedure*/
							}
						}
						getsymdo;
					}
				}
				else
				{
					if(sym==ifsym)     /*if statement*/
					{
					  int condition_type = -1;
					  // record the number of labels
					  int to_label = labelnum++;
						getsymdo;
						memcpy(nxtlev,fsys,sizeof(bool)*symnum);
						nxtlev[thensym]=true;
						nxtlev[dosym]=true;    /*can be followed by then or do*/
						conditiondo(nxtlev,ptx,lev, &condition_type);   /*call the condition (logical expression) parsing function*/
						// jump
            switch(condition_type) {
              case oddsym:
                  // take an instant number 2
                  fprintf(fa3, "\tli $t1, 2\n");
                  // divided by 2
                  fprintf(fa3, "\tdiv $t0, $t1\n");
                  // the remain is the highest 32 bit
                  fprintf(fa3, "\tmfhi $t2\n");
                  // jump when remain == 0, or continue
                  fprintf(fa3, "\tbeq $t2, $zero, label_%d\n", to_label);
                  break;
              case eql:
                  fprintf(fa3, "\tbne $t1, $t0, label_%d\n", to_label);
                  break;
              case neq:
                  fprintf(fa3, "\tbeq $t1, $t0, label_%d\n", to_label);
                  break;
              case lss:
                  fprintf(fa3, "\tbge $t1, $t0, label_%d\n", to_label);
                  break;
              case geq:
                  fprintf(fa3, "\tblt $t1, $t0, label_%d\n", to_label);
                  break;
              case gtr:
                  fprintf(fa3, "\tble $t1, $t0, label_%d\n", to_label);
                  break;
              case leq:
                  fprintf(fa3, "\tbgt $t1, $t0, label_%d\n", to_label);
                  break;
            }

						if(sym==thensym)
						{
							getsymdo;
						}
						else
						{
							error(16);          /*no "then" statement is found*/
						}
						cx1=cx;                /*save the address of current instruction*/
						gendo(jpc, 0, 0);			 /*generate the condition jump instruction; set the address to 0 for now*/

						statementdo(fsys,ptx,lev);   /*deal with the statements after then*/
						// create a label
						fprintf(fa3, "label_%d:\n", to_label);
						code[cx1].a=cx;      /*set the address of jun instruction.
																	after statement parsing, cx is where all statements of "then" is done,
																	which is the jump address we need*/
					}
					else
					{
						if(sym==beginsym)   /* parsing a complex sentence*/
						{
							getsymdo;
							memcpy(nxtlev,fsys,sizeof(bool)*symnum);
							nxtlev[semicolon]=true;
							nxtlev[endsym]=true;/*can be followed by ';' or end*/
							/* continously call statement parsing function until 'end', or the next symbol is not a beginnign symbol*/
							statementdo(nxtlev,ptx,lev);
							while(inset(sym,statbegsys)||sym==semicolon)
							{
								if(sym==semicolon)
								{
									getsymdo;
								}
								else
								{
									error(10);/*need ';'*/
								}
								statementdo(nxtlev,ptx,lev);
							}
							if(sym==endsym)
							{
								getsymdo;
							}
							else
							{
								error(17); /*need 'end' or ';'*/
							}
						}
						else
						{
							if(sym==whilesym)/*while statement*/
							{
							  int condition_type = -1;
							  // record the number of labels
                int start_label = labelnum++;
                int to_label = labelnum++;
								cx1=cx;        /*save the current position (condition statement)*/
								getsymdo;
								memcpy(nxtlev,fsys,sizeof(bool)*symnum);
								nxtlev[dosym]=true;/*can be followed by "do"*/


								// create label
                fprintf(fa3, "label_%d:\n", start_label);
								conditiondo(nxtlev,ptx,lev, &condition_type);  /*call condition (logical expression) parsing*/

								// conditional jump
                switch(condition_type) {
                    case oddsym:
                        // take an instant number 2
                        fprintf(fa3, "\tli $t1, 2\n");
                        // divided by 2
                        fprintf(fa3, "\tdiv $t0, $t1\n");
                        // the remain is the highest 32 bit
                        fprintf(fa3, "\tmfhi $t2\n");
                        // jump when remain == 0, or continue
                        fprintf(fa3, "\tbeq $t2, $zero, label_%d\n", to_label);
                        break;
                    case eql:
                        fprintf(fa3, "\tbne $t1, $t0, label_%d\n", to_label);
                        break;
                    case neq:
                        fprintf(fa3, "\tbeq $t1, $t0, label_%d\n", to_label);
                        break;
                    case lss:
                        fprintf(fa3, "\tbge $t1, $t0, label_%d\n", to_label);
                        break;
                    case geq:
                        fprintf(fa3, "\tblt $t1, $t0, label_%d\n", to_label);
                        break;
                    case gtr:
                        fprintf(fa3, "\tble $t1, $t0, label_%d\n", to_label);
                        break;
                    case leq:
                        fprintf(fa3, "\tbgt $t1, $t0, label_%d\n", to_label);
                        break;
                }

								cx2=cx;       /*save the next code address*/
								gendo(jpc,0,0);	/*generate a conditional jump code but leave the address 0 for now*/
								if(sym==dosym)
								{
									getsymdo;
								}
								else
								{
									error(18);      /*lacking "do"*/
								}
								statementdo(fsys,ptx,lev); /*loop body*/
								gendo(jmp,0,cx1);	/*check the condition again*/
								code[cx2].a=cx;   /*fill the address to jump off the loop, similar as in "if" statements*/

								// jump to start (unconditionally)
								fprintf(fa3, "\tj label_%d\n", start_label);
								// create a label of where to jump out of the loop
                fprintf(fa3, "label_%d:\n", to_label);
							}
							else
							{
								memset(nxtlev,0,sizeof(bool)*symnum);	/*nothing to add to the following set*/
								testdo(fsys,nxtlev,19);	/*test the statement's following set*/
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
/*
* expression parsing
*/
int expression(bool*fsys,int*ptx,int lev)
{
	enum symbol addop;                    /*minus or plus*/
	bool nxtlev[symnum];
	if(sym==plus||sym==minus)             /*if there is a minus or plus symbol, the expression is treated as a term*/
	{
		addop=sym;                    /*minus or plus*/
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);                /*term parsing*/
		if(addop==minus)
		{
			gendo(opr,0,1);                   /*if minus, generate a nagate instruction*/

			// negate a number
			// pop a number from the stack
      fprintf(fa3, "\tlw $t0, -4($sp)\n");
      // negate
      fprintf(fa3, "\tsub $t0, $zero, $t0\n");
      // push it back in
      fprintf(fa3, "\tsw $t0, -4($sp)\n");
		}
	}
	else                             /*the expression is regarded as term minus/plus another term*/
	{
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);            /*term parsing*/
	}
	while(sym==plus||sym==minus)
	{
		addop=sym;
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);              /*term parsing*/
		if(addop==plus)
		{
			gendo(opr,0,2);                    /*generate plus instruction*/
			// pop a number from the stack
      fprintf(fa3, "\tlw $t0, -4($sp)\n");
      // pop another number
      fprintf(fa3, "\tlw $t1, -8($sp)\n");
      // add
      fprintf(fa3, "\tadd $t2, $t1, $t0\n");
      // stack pointer --
      fprintf(fa3, "\taddi $sp, $sp, -4\n");
      // push back to the stack
      fprintf(fa3, "\tsw $t2, -4($sp)\n");
		}
		else
		{
      gendo(opr,0,3);                /*minus instruction*/
      // similar to adding
      fprintf(fa3, "\tlw $t0, -4($sp)\n");
      fprintf(fa3, "\tlw $t1, -8($sp)\n");
			// substraction
      fprintf(fa3, "\tsub $t2, $t1, $t0\n");
      fprintf(fa3, "\taddi $sp, $sp, -4\n");
      fprintf(fa3, "\tsw $t2, -4($sp)\n");
		}
	}
	return 0;
}
/*
* term parsing
*/
int term(bool*fsys,int *ptx,int lev)
{
    enum symbol mulop;               /*save the operator (+, -, *, /)*/
    bool nxtlev[symnum];
    memcpy(nxtlev,fsys,sizeof(bool)*symnum) ;
    nxtlev[times]=true;
    nxtlev[slash]=true;
    factordo(nxtlev,ptx,lev);       /*factor parsing*/
    while(sym==times||sym==slash)
    {
		mulop=sym;
        getsymdo;
        factordo(nxtlev,ptx,lev);
        if(mulop==times)
        {
            gendo(opr,0,4);          /*multiply instruction*/
            // pop a number from the stack
            fprintf(fa3, "\tlw $t0, -4($sp)\n");
            // pop another number
            fprintf(fa3, "\tlw $t1, -8($sp)\n");
            // multiply
            fprintf(fa3, "\tmult $t1, $t0\n");
            // use the lower 32 bit
            fprintf(fa3, "\tmflo $t2\n");
            // stack pointer --
            fprintf(fa3, "\taddi $sp, $sp, -4\n");
            // push back to the stack
            fprintf(fa3, "\tsw $t2, -4($sp)\n");
        }
        else
        {
            gendo(opr,0,5);           /*division instruction*/
            // similar to multiplying
            fprintf(fa3, "\tlw $t0, -4($sp)\n");
            fprintf(fa3, "\tlw $t1, -8($sp)\n");
            // divide
            fprintf(fa3, "\tdiv $t1, $t0\n");
            fprintf(fa3, "\tmflo $t2\n");
            fprintf(fa3, "\taddi $sp, $sp, -4\n");
            fprintf(fa3, "\tsw $t2, -4($sp)\n");
        }
    }
     return 0;
}
/*
* factor parsing
*/
int factor(bool*fsys,int *ptx,int lev)
 {
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys,fsys,24);           /*look for the symbol to start*/
    while(inset(sym,facbegsys))          /*until the symbol is not a start symbol*/
    {
      if(sym==ident)                   /*it's a const or variable?*/
      {
          i=position(id,*ptx);        /*look up symbol table*/
          if(i==0)
          {
              error(11);               /*no such identifier*/
          }
      else
      {
			switch(table[i].kind)
			{
					case constant:                                      /*symbol is a const?*/
						gendo(lit,0,table[i].val);                       /*push the value into stack*/
						// get the const's value
						fprintf(fa3, "\tli $t0, %d\n", table[i].val);
						// push into stack
						fprintf(fa3, "\tsw $t0, ($sp)\n");
						// stack pointer ++
						fprintf(fa3, "\taddi $sp, $sp, 4\n");
						break;
					case variable:                                      /*symbol is a variable?*/
						gendo(lod,lev-table[i].level,table[i].adr);      /*find the address and push into stack*/
						// get the variable's address
						fprintf(fa3, "\tlw $t0, var_%s\n", table[i].name);
						// push into stack
						fprintf(fa3, "\tsw $t0, ($sp)\n");
						// stack pointer++
						fprintf(fa3, "\taddi $sp, $sp, 4\n");
						break;
					case procedur:                                      /*symbol is a procedure?*/
						error(21);                                       /*cannot be a procedure*/
						break;
				}
			}
			getsymdo;
		}
		else
		{
			if(sym==number)                                             /*factor is a number*/
			{
				if(num>amax)
				{
					error(31);
					num=0;
				}
				gendo(lit,0,num);
				// create an instant number
        fprintf(fa3, "\tli $t0, %d\n", num);
        // into stack
        fprintf(fa3, "\tsw $t0, ($sp)\n");
        // stack pointer ++
        fprintf(fa3, "\taddi $sp, $sp, 4\n");
				getsymdo;
			}
			else
			{
				if(sym==lparen)                                      /*factor is an expression*/
				{
					getsymdo;
					memcpy(nxtlev,fsys,sizeof(bool)*symnum);
					nxtlev[rparen]=true;
					expressiondo(nxtlev,ptx,lev);
					if(sym==rparen)
					{
						getsymdo;
					}
					else
					{
						error(22);                                       /*lacking right parenthesis*/
					}
				}
				testdo(fsys,facbegsys,23);                        /*illegal symbol found following the factor*/
			}
		}
	}
	return 0;
}
/*condition parsing (logical expression parsing)*/
int condition(bool* fsys,int* ptx,int lev, int* condition_type)
{
    enum symbol relop;
    bool nxtlev[symnum];
    if(sym==oddsym)                        /*is odd*/
   	{
   	  // record condition_type
   	  *condition_type = oddsym;
			getsymdo;
			expressiondo(fsys,ptx,lev);
			gendo(opr,0,6);                              /*generate an odd instruction*/
			// get the value of expression to $t0. the value should be on the top of stack after calling expression function
			// pop a number from stack
			fprintf(fa3, "\tlw $t0, -4($sp)\n");
			// stack pointer --
			fprintf(fa3, "\taddi $sp, $sp, -4\n");
    }
    else
    {
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[eql]=true;
		nxtlev[neq]=true;
		nxtlev[lss]=true;
		nxtlev[leq]=true;
		nxtlev[gtr]=true;
		nxtlev[geq]=true;
		expressiondo(nxtlev,ptx,lev);
		if(sym!=eql&&sym!=neq&&sym!=lss&&sym!=leq&&sym!=gtr&&sym!=geq)
		{
			error(20);
		}
		else
		{

			relop=sym;
			getsymdo;
			expressiondo(fsys,ptx,lev);
			switch(relop)
			{
				case eql:
					gendo(opr,0,8);
					break;
				case neq:
					gendo(opr,0,9);
					break;
				case lss:
					gendo(opr,0,10);
					break;
				case geq:
					gendo(opr,0,11);
					break;
				case gtr:
					gendo(opr,0,12);
					break;
				case leq:
					gendo(opr,0,13);
					break;
			}
        // record condition_type
		    *condition_type = relop;
        // pop a number from the stack
        fprintf(fa3, "\tlw $t0, -4($sp)\n");
        // pop another number
        fprintf(fa3, "\tlw $t1, -8($sp)\n");
        // stack pointer --
        fprintf(fa3, "\taddi $sp, $sp, -8\n");
		}
    }
    return 0;
}                                                  

/*interpreter for the original compiler*/

void interpret()
{
	int p,b,t;             /*instruction pointer£¬base addres£¬stack pointer*/
	struct instruction i;  /*to store the current instruction*/
	int s[stacksize];      /*stack*/

	printf("start pl0\n");
	t=0;
	b=0;
	p=0;
	s[0]=s[1]=s[2]=0;

	do{
		i=code[p];         /*read the current instruction*/
		p++;
		switch(i.f)
		{
			case lit:        /*take the value of a (push into stack)*/
				s[t]=i.a;
				t++;
				break;
			case opr:        /*numerical/logical calculation*/
				switch(i.a)
				{
					case 0:
						t=b;
						p=s[t+2];
						b=s[t+1];

						break;
					case 1:
						s[t-1]=-s[t-1];
						break;
					case 2:
						t--;
						s[t-1]=s[t-1]+s[t];
						break;
					case 3:
						t--;
						s[t-1]=s[t-1]-s[t];
						break;
					case 4:
						t--;
						s[t-1]=s[t-1]*s[t];
						break;
					case 5:
						t--;
						s[t-1]=s[t-1]/s[t];
             			break;
					case 6:
						s[t-1]=s[t-1]%2;
						break;
					case 8:
						t--;
						s[t-1]=(s[t-1]==s[t]);
 						break;
					case 9:
						t--;
						s[t-1]=(s[t-1]!=s[t]);
 						break;
					case 10:
						t--;
						s[t-1]=(s[t-1]<s[t]);
 						break;
					case 11:
						t--;
						s[t-1]=(s[t-1]>=s[t]);
 						break;
					case 12:
						t--;
						s[t-1]=(s[t-1]>s[t]);
 						break;
					case 13:
						t--;
						s[t-1]=(s[t-1]<=s[t]);
 						break;
					case 14:
						printf("%d",s[t-1]);
						fprintf(fa2,"%d",s[t-1]);
						t--;
						break;
					case 15:
						printf("\n");
						fprintf(fa2,"\n");
						break;
					case 16:
						printf("?");
						fprintf(fa2,"?");
						scanf("%d",&(s[t]));
						fprintf(fa2,"%d\n",s[t]);
						t++;
						break;
				}
				break;
			case lod:       /*take a value with the address of (current procedure's base address) + a and push into stack*/
				s[t]=s[base(i.l,s,b)+i.a];
				t++;
				break;
			case sto:       /*pop a number from stack and store to the address of (current procedure's base address) + a*/
				t--;
	            s[base(i.l,s,b)+i.a]=s[t];
				break;
			case cal:              /*call subprogram*/
				s[t]=base(i.l,s,b); /*push into stack the father program's base address*/
				s[t+1]=b;           /*push into stack the current program's base address*/
				s[t+2]=p;           /*push into stack the current instruction pointer*/
				b=t;                /*change base address pointer too the new procedure's base address*/
				p=i.a;              /*jump*/
				break;
			case inte:             /*allocate memory*/
				t+=i.a;
				break;
			case jmp:             /*unconditional jump*/
				p=i.a;
				break;
			case jpc:              /*conditional jump*/
				t--;
				if(s[t]==0)
				{
					p=i.a;
				}
	    		break;
		}
	}while (p!=0);

}

/*get the previous level's procedure's base address*/
int base(int l,int * s,int b)
{
	int b1;
	b1=b;
	 while(l>0)
	 {
		 b1=s[b1];
     	 l--;
	 }
	 return b1;
}


