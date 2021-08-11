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
int negative_charge_cline[51];
#include"input.hpp"
void calc_particle_complex_dir();
void input_and_calc_init(){
    float max_charge=0;
    input_particle();
    for(int i=1;i<=ptc_num;i++){
        max_charge=max(max_charge,abs(particle[i].charge));
        charge_sum+=abs(particle[i].charge);
    }
    for(int i=1;i<=ptc_num;i++){
            particle[i].line_num=ceil(line_num*abs(particle[i].charge)/max_charge);
            if(particle[i].charge<0) continue;
            if(particle[i].line_num==1) particle[i].line_num=4;
            if(particle[i].line_num&1) particle[i].line_num++;
    }
    calc_particle_complex_dir();
    return;
}
void calc_particle_complex_dir(){
    for(int i=1;i<=ptc_num;i++){
        negative_charge_cline[i]=0;
        if(particle[i].charge<0) continue;
        particle[i].rl=cos(Pi*2/particle[i].line_num);
        particle[i].il=sin(Pi*2/particle[i].line_num);
    }
}
void calc(){
    is_lineto_neg[0]=0.5;
    conpoint_num=0; que_len=0;
    calc_particle_complex_dir();
    float BROAD=max(1/zoom,1/TOTAL_zoom);
    for(int i=1;i<=ptc_num;i++){
        if(particle[i].charge<0) continue;
        float step_length=0.01*MIN_BROAD_LEFT/TOTAL_zoom;
        particle[i].mx=step_length;
        particle[i].my=0;
        float pre_movex=0.0,pre_movey=0.0;
        for(int p=1;p<=particle[i].line_num;p++){
            int pre_num=conpoint_num;
            list[++conpoint_num].x=particle[i].x+particle[i].mx;
            list[conpoint_num].y=particle[i].y+particle[i].my;
            float pre_x=list[conpoint_num].x,pre_y=list[conpoint_num].y;
            float rmx=particle[i].mx,rmy=particle[i].my;
            particle[i].mx=rmx*particle[i].rl-rmy*particle[i].il,
            particle[i].my=particle[i].rl*rmy+particle[i].il*rmx;
            for(;;){
                if(pre_x>BROAD||pre_x<-BROAD||pre_y>BROAD||pre_y<-BROAD) break;
                     //out of board
                float Es_x=0,Es_y=0;
                bool is_near_negative_ptc=0;
                for(int j=1;j<=ptc_num;j++){
                    float dist_sqr=(particle[j].x-pre_x)*(particle[j].x-pre_x)+
                                   (particle[j].y-pre_y)*(particle[j].y-pre_y);
                    float dist=sqrt(dist_sqr);
                    float particle_dist=sqrt((particle[j].x-particle[i].x)*(particle[j].x-particle[i].x)+
                                             (particle[j].y-particle[i].y)*(particle[j].y-particle[i].y));
                    if(step_length<particle_dist/3){
                        if(dist<step_length&&particle[j].charge<0){
                            is_near_negative_ptc=1;
                            negative_charge_cline[j]++;
                            break;
                        }
                    }
                    float E=k*particle[j].charge/dist_sqr;
                    Es_x+=E/dist*(pre_x-particle[j].x);
                    Es_y+=E/dist*(pre_y-particle[j].y);
                }
                if(conpoint_num+1>50000){
                    printf("OUT OF MEMORY WARNING!\n"); exit(-1);
                }
                if(is_near_negative_ptc){is_lineto_neg[que_len]=1.0; break;}
                float slope=Es_y/Es_x;                  //move control point
                float movex=(Es_x>0?1:-1)/sqrt(1.0+slope*slope),movey=(Es_y>0?1:-1)*sqrt(1-movex*movex);
                movex*=step_length,movey*=step_length;
                if(abs(pre_movex+movex)<step_length/1e5&&abs(pre_movey+movey)<step_length/1e5){
                    conpoint_num=pre_num; break;
                }
                list[++conpoint_num].x=pre_x+movex,list[conpoint_num].y=pre_y+movey;
                pre_x=list[conpoint_num].x; pre_y=list[conpoint_num].y;             //move control point
                pre_movey=movey; pre_movex=movex;
            }
            if(conpoint_num>pre_num){
                line_que[++que_len]=conpoint_num;
                is_lineto_neg[que_len]=0.5;
            }
        }
    }
    printf("%d %.6lf %.6lf %.6lf\n",conpoint_num,BROAD,zoom,user_zoom);
    return;
}
void init_calc(){
    input_and_calc_init();
    calc();
    return;
}