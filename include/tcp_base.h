
#ifndef __TCP_BASE_H__
#define __TCP_BASE_H__


#ifdef __TCP_BASE_C__
#define __TCP_BASE_EXT__
#else
#define __TCP_BASE_EXT__	extern
#endif



__TCP_BASE_EXT__ void Connect_Server(char *string);
__TCP_BASE_EXT__ void Rec_Server_Data(void);
__TCP_BASE_EXT__ void tcp_heart_beat(void);





#endif 


