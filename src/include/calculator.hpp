#define MAX_CONTROLPOINT_NUM 1000020            //allowed control point's amount
#define k 9e9               //Electrostatic force constant 
#define Pi acos(-1)
struct electricity_spot{    //electricity particle
    float x,y,charge;       //location and charge
    float mx=0.01,my;       //which direction the control point shoule move first
    float rl,il;            //here i use complex number to turn the moving direction
    int line_num;           //how many line this particle connecting
}particle[51];
int ptc_num;
struct ctrlpoint{           //control point
    float x,y;
}list[MAX_CONTROLPOINT_NUM];
int conpoint_num;
int line_num,que_len;
int line_que[510];          //line of force
float is_lineto_neg[510];       //is electricty line connect to negative charge
float charge_sum=0;
void input_and_calc_init(){
    float max_charge=0;
    scanf("%d",&ptc_num);
    cin>>line_num;
    for(int i=1;i<=ptc_num;i++){
        scanf("%f %f %f",&particle[i].x,&particle[i].y,&particle[i].charge);
        max_charge=max(max_charge,particle[i].charge);
        charge_sum+=abs(particle[i].charge);
    }
    for(int i=1;i<=ptc_num;i++){
        if(particle[i].charge>0){
            particle[i].line_num=ceil(line_num*particle[i].charge/max_charge);
            if(particle[i].line_num==1) particle[i].line_num=4;
            if(particle[i].line_num&1) particle[i].line_num++;
        }
    }
    for(int i=1;i<=ptc_num;i++){
        if(particle[i].charge<0) continue;
        particle[i].rl=cos(Pi*2/particle[i].line_num);
        particle[i].il=sin(Pi*2/particle[i].line_num);
    }
    return;
}
void calc(){
    input_and_calc_init();
    is_lineto_neg[0]=0.5;
    for(int i=1;i<=ptc_num;i++){
        if(particle[i].charge<0) continue;
        for(int p=1;p<=particle[i].line_num;p++){
            if(i==2&&p==1){
                p++; p--;
            }
            int pre_num=conpoint_num;
            list[++conpoint_num].x=particle[i].x+particle[i].mx;
            list[conpoint_num].y=particle[i].y+particle[i].my;
            float pre_x=list[conpoint_num].x,pre_y=list[conpoint_num].y;
            float rmx=particle[i].mx,rmy=particle[i].my;
            particle[i].mx=rmx*particle[i].rl-rmy*particle[i].il,
            particle[i].my=particle[i].rl*rmy+particle[i].il*rmx;
            //printf("%.2lf %.2lf\n",mx,my);
            // if(particle[3].line_num!=20){
            //     p++;p--;
            // }
            for(;;){
                if(pre_x>2||pre_x<-2||pre_y>2||pre_y<-2) break;     //out of board
                float Es_x=0,Es_y=0;
                bool is_near_negative_ptc=0;
                for(int j=1;j<=ptc_num;j++){
                    // if(i==2&&p==1&&j==3){
                    //     j++;j--;
                    // }
                    float dist_sqr=(particle[j].x-pre_x)*(particle[j].x-pre_x)+
                                   (particle[j].y-pre_y)*(particle[j].y-pre_y);
                    float dist=sqrt(dist_sqr);
                    if(dist<1e-2&&particle[j].charge<0) {is_near_negative_ptc=1; break;}
                    float E=k*particle[j].charge/dist_sqr;
                    Es_x+=E/dist*(pre_x-particle[j].x);
                    Es_y+=E/dist*(pre_y-particle[j].y);
                }
                if(is_near_negative_ptc){is_lineto_neg[que_len]=1.0; break;}
                if(sqrt(Es_x*Es_x+Es_y*Es_y)/(charge_sum)<1e8){
                    conpoint_num=pre_num; break;
                }
                float slope=Es_y/Es_x;                  //move control point
                float movex=(Es_x>0?1:-1)/sqrt(1.0+slope*slope),movey=(Es_y>0?1:-1)*sqrt(1-movex*movex);
                movex*=0.01,movey*=0.01;
                list[++conpoint_num].x=pre_x+movex,list[conpoint_num].y=pre_y+movey;
                pre_x+=movex; pre_y+=movey;             //move control point
            }
            if(conpoint_num>pre_num){
                line_que[++que_len]=conpoint_num;
                is_lineto_neg[que_len]=0.5;
            }
            // printf("%d %d\n",p,conpoint_num-pre_num);
        }
    }
    // printf("%d\n",conpoint_num);
    // system("pause");
    return;
}