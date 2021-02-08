#ifndef __XMSLIB_H

extern  unsigned  InitXMS();
extern  unsigned  XMSVersion();
extern  unsigned  AllocXMS(unsigned);
extern  unsigned  FreeXMS(unsigned);
extern  unsigned  StoreXMS(unsigned,char far*,unsigned long,unsigned long);
extern  unsigned  LoadXMS(unsigned,char far*,unsigned long,unsigned long);

#endif
