inline void input_particle(){
    scanf("%d",&ptc_num);
    cin>>line_num;
    for(int i=1;i<=ptc_num;i++){
        scanf("%f %f %f",&particle[i].x,&particle[i].y,&particle[i].charge);
        float tmp=min(MIN_BROAD_LEFT/abs(particle[i].x),MIN_BROAD_LEFT/abs(particle[i].y));
        zoom=min(zoom,tmp);
    }
    return;
}