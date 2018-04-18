#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
//Working
//---------------------------------------------------------------------------------------
enum state {IF, ID, IS, EX, WB};
int clock = 0;
int c=2000;
int count=0;
int number_of_cycles;
int number_of_instructions;
int zz=0;
int ii=0;
int jj=0;
int end_of_file_reached=0;
FILE *fp;
//---------------------------------------------------------------------------------------
struct fake_rob
{
    int stage;
    int tag;
    int type, dest, src1, src2;
    int completed;
    int IF_t, ID_t, IS_t, EX_t, WB_t, FR_t;
    int dest_tag;
};

struct fake_rob rob[10024];
struct fake_rob *start_rob;
struct fake_rob *end_rob;
//---------------------------------------------------------------------------------------
struct dispatch_list
{
    int tag;
    int stage;
};

struct dispatch_list *dispatch;
struct dispatch_list *start_fetch;
struct dispatch_list *end_fetch;
struct dispatch_list *start_dispatch;
struct dispatch_list *end_dispatch;
struct dispatch_list temp_d[10024];
//----------------------------------------------------------------------------------------
struct issue_list
{
    int tag;
    int src1_ready, src2_ready;
    int src1_tag, src2_tag;
};

struct issue_list *issue;
struct issue_list *start_issue;
struct issue_list *end_issue;
struct issue_list temp[1000];
//----------------------------------------------------------------------------------------
struct execute_list
{
    int tag;
 S = atoi(argv[1]);
    N = atoi(argv[2]);
    blocksize = atoi(argv[3]);
    L1_size = atoi(argv[4]);
    L1_assoc = atoi(argv[5]);
    L2_size = atoi(argv[6]);
    L2_assoc = atoi(argv[7]);
    strcpy(trace_file, argv[8]);
    int cycle;
}execute_list;
struct execute_list execute[15000];
struct execute_list *start_execute;
struct execute_list *end_execute;
//----------------------------------------------------------------------------------------
struct register_file
{
    int tag;
    int valid;
    //int rename;
};

struct register_file rmt[128];
//-----------------------------------------------------------------------------------------
void AdvanceCycle()
{

    int pc, operation, dest_reg, src1_reg, src2_reg, mem_address;
    number_of_cycles = clock;
    clock++;
    if( end_of_file_reached && ii == jj)
    {
        clock = 0;
    }

}
//-----------------------------------------------------------------------------------------
void update_rob(int tag, int st)
{
//printf("rob update hua\n");
    int i;
    for (i=0; i<10024; i++)
    {
        if(tag == rob[i].tag)
        {
            rob[i].stage = st;
//	    printf("My state %d is setting clock %d\n",st,clock);
            if(rob[i].stage == IF)
            {
                rob[i].IF_t = clock;
                break;
            }
            else if(rob[i].stage == ID)
            {
                rob[i].ID_t = clock;
                break;
            }
            else if(rob[i].stage == IS)
            {
                rob[i].IS_t = clock;
                break;
            }
            else if(rob[i].stage == EX)
            {
                rob[i].EX_t = clock;
                break;
            }
            else if(rob[i].stage == WB)
            {
                rob[i].WB_t = clock;
                rob[i].completed = 1;
                break;
            }
        }
    }
}
//-----------------------------------------------------------------------------------------
void FakeRetire()
{
    //printf("retire la aala at clock %d\n",clock);
    int j;
    while((start_rob != end_rob) && start_rob->stage == WB)
    {
        start_rob->FR_t = clock;
        printf("%d fu{%d} src{%d,%d} dst{%d} IF{%d,%d} ID{%d,%d} IS{%d,%d} EX{%d,%d} WB{%d,%d}\n", start_rob->tag, start_rob->type, start_rob->src1, start_rob->src2, start_rob->dest, start_rob->IF_t, (start_rob->ID_t - start_rob->IF_t), start_rob->ID_t, (start_rob->IS_t - start_rob->ID_t), start_rob->IS_t, (start_rob->EX_t - start_rob->IS_t), start_rob->EX_t, (start_rob->WB_t - start_rob->EX_t), start_rob->WB_t, 1);
	//printf("#################  My new start rob is at tag %d\n ################### My end rob is at tag %d \n",start_rob->tag, end_rob->tag);
        ii++;
        if(ii > 10023)
            {
                start_rob = rob;
                ii = 0;
            }
//	printf("             ROb tag is %d and dest %d\n",start_rob->tag, start_rob->dest_tag );
        for (j=0; j<128; j++)
            {
//		if ( rmt[j].tag > 0 )
//			printf(" Sample rmt value %d\n",rmt[j].tag);
                if (rmt[j].tag == start_rob->dest_tag && rmt[j].valid == 0)
                {
//		    printf("         Setting valid bit for %d\n",rmt[j].tag);
                    rmt[j].valid = 1;
                }
            }
        start_rob++;
    }
}
//-----------------------------------------------------------------------------------------
void Execute(int N, int S)
{
//printf("Execute la aala\n");
    int i, j,k, issue_itr, itr;
	    for( itr =0 ; itr < 10024 ; itr++ ){
            for (issue_itr = 0 ; issue_itr < S ; issue_itr ++ ){
		if ( rob[itr].completed == 1 && rob[itr].stage ==WB){
	//		printf("&&&&&&&&&&&&&&&&&&   itr %d .. issue itr : %d     \n",itr,issue[issue_itr].tag);
			if ( rob[itr].dest_tag == issue[issue_itr].src1_tag ){
	//			printf("&&&&&&&&&&&&&&&&& Setting src1 for offset %d with dest value %d\n",issue[issue_itr].tag,rob[itr].dest_tag);
				issue[issue_itr].src1_ready=1;
			}
			if ( rob[itr].dest_tag == issue[issue_itr].src2_tag ){
	//			printf("&&&&&&&&&&&&&&&&&&&&  Setting src2 for offset %d with dest value %d\n",issue[issue_itr].tag,rob[itr].dest_tag);
				issue[issue_itr].src2_ready=1;
			}

		}
	    }
    }
    for (i=0; i<15000; i++)
    {
        if(execute[i].cycle>0  && execute[i].tag >= 0){
            execute[i].cycle--;
	    //printf("/........ Offset %d.... reducing cycle to %d\n",execute[i].tag,execute[i].cycle);
	}
        if(execute[i].cycle == 0 && execute[i].tag >= 0 )
        {
            //**********************************************                                  CHECK
            /*rob[execute.tag].stage = WB;
            rob[execute.tag].WB_t = clock;*/
		//printf("Setting offset :%d to state %d\n",execute[i].tag,WB);
            update_rob(execute[i].tag, WB);
	// Loop that will go and update src1 , src2 ready for all data in issue[] queue
            for (j=0; j<S; j++)
            {
                /*if (issue[j].src1_tag == execute[i].tag && execute[i].tag>=0)/////////////////////////////PROBLEM       REWRITE execute[i].tag_dest
                {
		    printf (" Src1 ready set to ready at offset : %d  ALU(i) is : %d and index j is %d \n",issue[j].src1_tag,i, j) ;
                    issue[j].src1_ready = 1;
                }
                else if(issue[j].src2_tag == execute[i].tag && execute[i].tag>=0)
                {
		    printf (" Src2 ready set to ready at offset : %d\n",issue[j].src1_tag) ;
                    issue[j].src2_ready = 1;
                }*/
                for (k=0; k<10024; k++)
                {
                    if(execute[i].tag == rob[k].tag && rob[k].tag >=0)
                    {
	//		printf(" execute[i].tag : %d \n",execute[i].tag);
                        if(issue[j].src1_ready==0 && issue[j].src1_tag==rob[k].dest_tag)
                        {
	//			printf(" .................. setting src1: %d\n", issue[j].tag);
                            issue[j].src1_ready = 1;
                        }
                        if(issue[j].src2_ready==0 && issue[j].src2_tag==rob[k].dest_tag)
                        {
	//			printf("------------------------ setting src2: %d\n", issue[j].tag);
                            issue[j].src2_ready = 1;
                        }
                    }
                }
            }

            execute[i].tag = -1;
        }
    }
}
//-----------------------------------------------------------------------------------------
void Issue(int N, int S)
{
	//printf("Issue la aala\n");
    int i,j,k;
    struct issue_list swapper;

    for(j=0; j<1000; j++)
    {
        temp[j].tag = -2;
    }

    i = 0;
    for (j=0; j<S; j++)
    {
	//if ( issue[j].tag ==26 ){
//	    printf(" Checking 26 operands ready %d and %d\n",issue[j].src1_ready,issue[j].src2_ready);
	//}
        if(issue[j].src1_ready==1 && issue[j].src2_ready==1 && issue[j].tag>=0)
        {
//	    printf(" Copy to temp queue with tag offset %d\n",issue[j].tag);
            memcpy(&temp[i],&issue[j],sizeof(temp[i]));
            i++;
        }
    }

	//printf("1 \n");
    for(k=0; k<(i-1); k++)
    {
        for(j=0; j<i-1-k; j++)
        {
            if(temp[j].tag > temp[j+1].tag)
            {
		memset(&swapper,0,sizeof(swapper));
                memcpy(&swapper, &temp[j],sizeof(swapper));
                memcpy(&temp[j],&temp[j+1],sizeof(swapper));
                memcpy(&temp[j+1],&swapper,sizeof(swapper));

            }
        }
    }
	//printf("2 \n");

    for(i=0; i<N; i++)
    {
        if (temp[i].tag >= 0)
        {
            end_execute->tag = temp[i].tag;
            for(k=0; k<15000; k++)
            {
                if(temp[i].tag == rob[k].tag)
                {
                    if(rob[k].type == 0)
                        end_execute->cycle = 1;
                    else if(rob[k].type == 1)
                        end_execute->cycle = 2;
                    else if(rob[k].type == 2)
                        end_execute->cycle = 5;
                    break;
                }
            }
            end_execute++;
            zz++;
            if(zz > 15000)
            {
                end_execute = execute;
                zz = 0;
            }

        }
    }

    //printf("3 \n");
    for(i=0; i<N; i++)
    {
        for(k=0; k<S; k++)
        {
	//	if ( temp[i].tag > 0 )
	//		printf("Checking values temp: %d and %d\n",temp[i].tag,issue[k].tag);
            if(temp[i].tag == issue[k].tag && issue[k].tag >= 0 )
            {
		//printf("Setting offset :%d to state %d \n",temp[i].tag,3);
                update_rob(issue[k].tag,3);
                issue[k].tag = -1;
            }
        }
    }
	//printf("4 \n");
}
//-----------------------------------------------------------------------------------------
void Dispatch(int N, int S)
{
//printf("Dispatch la aala\n");
    int i,j,k,z,m;


    for(j=0; j<10024; j++)
    {
        temp_d[j].tag = -2;
    }

    i = 0;
    for(j=0; j<(2*N); j++)
    {
        if(dispatch[j].stage == ID)
        {
 S = atoi(argv[1]);
    N = atoi(argv[2]);
    blocksize = atoi(argv[3]);
    L1_size = atoi(argv[4]);
    L1_assoc = atoi(argv[5]);
    L2_size = atoi(argv[6]);
    L2_assoc = atoi(argv[7]);
    strcpy(trace_file, argv[8]);
            memcpy(&temp_d[i],&dispatch[j],sizeof(dispatch[j])) ;
	   // printf("My tag is %d\n",temp_d[i].tag);

            i++;
        }
    }

    for(k=0; k<(i-1); k++)
    {
        for(j=0; j<i-1-k; j++)
        {
            if(temp_d[j].tag > temp_d[j+1].tag)
            {
                temp_d[10023] = temp_d[j];
                temp_d[j] = temp_d[j+1];
                temp_d[j+1] = temp_d[10023];
            }
        }
    }

    j=0;
    for(i=0; i<S; i++)
    {
        if(issue[i].tag == -1 && temp_d[j].tag >= 0)
        {
            issue[i].tag = temp_d[j].tag;
	   // printf("Setting offset :%d to state %d\n",issue[i].tag,2);
            update_rob(issue[i].tag,2);

            for(z=0; z<(2*N); z++)
            {
                if(dispatch[z].tag == temp_d[j].tag){
                    dispatch[z].tag = -1;
                    dispatch[z].stage = -1;
		}
            }

            for(k=0; k<10024; k++)
            {

                if(temp_d[j].tag == rob[k].tag && rob[k].tag>=0)
                {
                    if(rob[k].src1 < 0 || ( rob[k].src1 >= 0 && rmt[rob[k].src1].valid == 1))
                    {
                        issue[i].src1_ready = 1;
                    }
                    else
                    {
                        issue[i].src1_ready =0;
                        issue[i].src1_tag = rmt[rob[k].src1].tag;
                    }
                    if( rob[k].src2 < 0 || (rob[k].src2 >= 0 && rmt[rob[k].src2].valid == 1))
                    {
                        issue[i].src2_ready = 1;
                    }
                    else
                    {
                        issue[i].src2_ready =0;
                        issue[i].src2_tag = rmt[rob[k].src2].tag;
                    }

                    if(rob[k].dest >= 0){
                        rmt[rob[k].dest].valid = 0;//########################################Problem
                        rmt[rob[k].dest].tag = c;//##########################################Check
                        rob[k].dest_tag = c;
           //             printf("****************************************rmt[rob[k].dest].tag is %d rob[k].dest is %d at index %d\n", rmt[rob[k].dest].tag, rob[k].dest_tag,rob[k].dest);
                        c++;
                        break;
                    }
                }
            }
            j++;
            if(j == N)
                break;
        }
    }

    for(i=0; i<(2*N); i++)
    {
        if(dispatch[i].stage == IF)
        {
            dispatch[i].stage = ID;
	//	printf("Setting offset :%d to state %d\n",dispatch[i].tag,1);
            update_rob(dispatch[i].tag,1);
        }
    }
}
//-----------------------------------------------------------------------------------------
void Fetch(int N, int S)
{
    int pc, operation, dest_reg, src1_reg, src2_reg, mem_address;
    int i,j,z;
    j=0;

  //  printf("Inside fetch for clock %d\n",clock);
    for(i=0; i<(2*N); i++)
    {
        //if(dispatch[i].tag == -1)
        //{
            while( j<N && dispatch[i].tag==-1){
            	if ( fscanf(fp,"%x %d %d %d %d %x",&pc,&operation,&dest_reg,&src1_reg,&src2_reg,&mem_address) == EOF ){
			end_of_file_reached=1;
			break;
		}
                number_of_instructions++;
                dispatch[i].tag = count;
                dispatch[i].stage = IF;
                end_rob->tag = count;
                end_rob->stage = IF;
                end_rob->IF_t = clock;
                end_rob->type = operation;
                end_rob->dest = dest_reg;
                end_rob->src1 = src1_reg;
                end_rob->src2 = src2_reg;
                j++;
            //    printf("end_rob->tag value is %d\n",end_rob->tag);
                end_rob++;
                jj++;
                if(jj>10023)
                {
                    jj = 0;
                    end_rob = rob;
                }
                count++;
            }
        //}
    }

}
//-----------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    int S, N, blocksize, L1_size, L1_assoc, L2_size, L2_assoc;
    char trace_file[30];
    int i,z;
    float IPC;

    S = atoi(argv[1]);
    N = atoi(argv[2]);
    blocksize = atoi(argv[3]);
    L1_size = atoi(argv[4]);
    L1_assoc = atoi(argv[5]);
    L2_size = atoi(argv[6]);
    L2_assoc = atoi(argv[7]);
    strcpy(trace_file, argv[8]);


//------------------------------------------------------------------------------------------
    dispatch = (struct dispatch_list *) malloc (sizeof (struct dispatch_list) * (2*N));

    issue = (struct issue_list *) malloc (sizeof (struct issue_list) * S);

    //execute = (struct execute_list *) malloc (sizeof (struct execute_list * N));
//------------------------------------------------------------------------------------------
    for (i=0; i<10024; i++)
    {
        rob[i].tag = -1;
        rob[i].completed = 0;
        rob[i].dest_tag = -5;
    }

    for (i=0; i<(2*N); i++)
    {
        dispatch[i].tag = -1;
        dispatch[i].stage = -1;
    }

    for (i=0; i<S; i++)
    {
        issue[i].tag = -1;
    }

    for (i=0; i<15000; i++)
    {
        execute[i].tag = -1;
        execute[i].cycle = -1;
    }

    for (i=0; i<128; i++)
    {
        rmt[i].valid = 1;
    }
//----------------------------------------------------------------------------------------------
    start_rob = rob;
    end_rob = rob;

    start_fetch = dispatch;
    end_fetch = dispatch;
    //start_dispatch = &dispatch[N];
    //end_dispatch = &dispatch[N];

    start_issue = issue;
    end_issue = issue;

    start_execute = execute;
    end_execute = execute;
//printf("allocation jhala\n");
    fp = fopen(trace_file, "r");
//-----------------------------------------------------------------------------------------------
    do{
            /*if(clock == 322 ||clock == 323 || clock==324)
            //{
                //for(i=0;i<S;i++){
                  //  printf("Issue cha tag %d src1_ready %d src1_tag %d src2_ready %d src2_tag %d \n",issue[i].tag, issue[i].src1_ready, issue[i].src1_tag, issue[i].src2_ready, issue[i].src2_tag);
                  //  if(issue[i].tag == 1136)
                   // {
                     //   for(z=0;z<10024;z++)
                       // {
                        //    if(issue[i].tag == rob[z].tag)
                         //   {
                          //      printf("%d\n",rob[z].dest_tag);
                           // }
                        //}
                   // }
                //}
                for(i=0; i<2*N; i++){
                    printf("Dospatch cha tag %d\n", dispatch[i].tag);
                }
            //}*/
        /*for(i=0;i<10024;i++)
        {
            if(rob[i].tag == 2048 || rob[i].tag == 2047)
            {
                printf("@@@@@@@@@@@@@@@@%d\n",rob[i].stage);
                printf("#######start_rob %d end_rob %d\n", start_rob->tag, end_rob->tag);
            }
        }*/

        FakeRetire();
        Execute(N,S);
        Issue(N,S);
        Dispatch(N,S);
        Fetch(N,S);
        //printf("%d CLOCK Cycle ended here\n",clock);
        AdvanceCycle();
      } while (clock);

    IPC = (float)number_of_instructions/number_of_cycles;
    printf("CONFIGURATION\n");
    printf("superscalar bandwidth (N) = %d\n",N);
    printf("dispatch queue size (2*N) = %d\n", 2*N);
    printf("schedule queue size (S) = %d\n",S);
    printf("RESULTS\n");
    printf("Number of instructions = %d\n",number_of_instructions);
    printf("Number of cycles = %d\n",number_of_cycles);
    printf("IPC = %.2f\n", IPC);
    //for(i=0; i<2*N; i++)
      //  printf("%d\n",dispatch[i].stage);
    fclose(fp);
    return 0;
 S = atoi(argv[1]);
    N = atoi(argv[2]);
    blocksize = atoi(argv[3]);
    L1_size = atoi(argv[4]);
    L1_assoc = atoi(argv[5]);
    L2_size = atoi(argv[6]);
    L2_assoc = atoi(argv[7]);
    strcpy(trace_file, argv[8]);
}
