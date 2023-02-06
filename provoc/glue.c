#define FALSE 0
#define TRUE 1
#ifndef NULL
#define NULL 0
#endif

#define IEC_USINT unsigned short
#define IEC_SINT short
#define IEC_DINT int
#define IEC_INT short
#define IEC_LINT long long
#define IEC_UDINT unsigned int
#define IEC_UINT  unsigned short
#define IEC_ULINT unsigned long long
#define IEC_REAL  float
#define IEC_BOOL  unsigned char
#define IEC_WORD unsigned short
#define IEC_DWORD unsigned int
#define IEC_BYTE unsigned char
typedef struct _times {
    int microsec;
}iec_t;
iec_t __time_to_timespec(int flg, int day, int hour, int mi, int sec, int ms);
iec_t  __date_to_timespec(int day, int month, int year);
//Special
#define TIME_TO_DINT(sw,null,exp)  (int)((exp).microsec)
#define DINT_TO_TIME(sw,null,exp)  __time_to_timespec(1,0,0,0,0,(exp));//expasion point reloc
#define EQ_IEC_TIME(omiss, ecl, num, p0, p1) (p0).microsec == (p1).microsec
#define DATE_TO_DINT(sw,null,exp)  (int)((exp).microsec)
#define DINT_TO_DATE(sw,null,exp)  __date_to_timespec(0,0,(exp));//expasion point reloc
#define EQ_IEC_DATE(omiss, ecl, num, p0, p1) (p0).microsec == (p1).microsec
#define IEC_TIME iec_t
#define IEC_DATE iec_t
//Normal
#define WORD_TO_INT(sw,null,exp)     (IEC_INT)(exp)
#define WORD_TO_BOOL(sw,null,exp)    (IEC_BOOL)(exp)
#define BYTE_TO_INT(sw,null,exp)     (IEC_INT)(exp)
#define BOOL_TO_BYTE(sw,null,exp)    (IEC_BYTE)(exp)
#define WORD_TO_INT(sw,null,exp)     (IEC_INT)(exp)
#define WORD_TO_DWORD(sw,null,exp)   (IEC_DWORD)(exp)
#define BYTE_TO_BOOL(sw,null,exp)    (IEC_BOOL)(exp)
#define INT_TO_BYTE(sw,null,exp)     (IEC_BYTE)(exp)
#define DINT_TO_UDINT(sw,null,exp)   (IEC_UDINT)(exp)
#define REAL_TO_WORD(sw,null,exp)    (IEC_WORD)(exp)
#define REAL_TO_INT(sw,null,exp)     (IEC_INT)(exp)
#define BOOL_TO_WORD(sw,null,exp)    (IEC_WORD)(exp)
#define BYTE_TO_DWORD(sw,null,exp)   (IEC_DWORD)(exp)
#define DWORD_TO_REAL(sw,null,exp)   (IEC_REAL)(exp)
#define BOOL_TO_INT(sw,null,exp)     (IEC_INT)(exp)


#define extIEC_SINT 3
#define extIEC_INT 3
#define extIEC_UINT 3
#define extIEC_DINT 0
#define extIEC_UDINT 0
#define extIEC_BOOL 1
#define extIEC_WORD 3
#define extIEC_REAL 2
#define extIEC_DWORD 0
#define extIEC_BYTE 1
#define extIEC_TIME 2 //RTS鏈敞鍐?
#define extIEC_ULINT 0
#define extIEC_DATE 0
#define extIEC_USINT 3
#define extIEC_LINT 0


#define ll unsigned long long
typedef  void* (*Handle) (int);
typedef  void  (*Insert) (int, unsigned long long p, int);
typedef  int   (*Find)   (unsigned long long p);
typedef  void  (*Assign) (void);
typedef  void  (*DInsert)(int, char, int, char, int);//UID, domain, offset, step, len
typedef  ll    (*pcode)  (unsigned char* a);
typedef  void  (*pderl)  (unsigned char* a);

Handle  handle       __attribute__((__section__(".bss")));
Insert  insert       __attribute__((__section__(".bss")));
Find    find         __attribute__((__section__(".bss")));
Assign  dispos       __attribute__((__section__(".bss")));
DInsert dinsert      __attribute__((__section__(".bss")));

#define RelocPointF(type,var)\
    (*((type*)((ll)(&(var)) + a)))

void DirecLoc(unsigned char* a);
ll CodeInit(unsigned char* a);
ll Entry(unsigned char* a, Handle h, Insert i, Find f, Assign d,DInsert d_i) {
    RelocPointF(Handle, handle) = h;
    RelocPointF(Insert, insert) = i;
    RelocPointF(Find, find) = f;
    RelocPointF(Assign, dispos) = d;
    RelocPointF(DInsert, dinsert) = d_i;
    ((pderl)((ll)DirecLoc + a))(a);
    ll tmp_add = ((pcode)((ll)CodeInit + a))(a);
    return tmp_add;
}

void memcpy(void *dst,void *src, int len)
{
	 int i;
	 for(i =0; i <len; i ++)
	 {
		 *((unsigned char *)dst+i)=*((unsigned char *)src+i);
	 }	 
	
}
//TYPE TRANS
//动态添加
typedef unsigned char (*ptrans)(IEC_BOOL, unsigned char*, IEC_BYTE, IEC_SINT);
//
#define iec_trans(fname,in1,in2,op,...)\
    in1 ins_##fname##__##in1##__##in1##__##in2 (IEC_BOOL p0,unsigned char *p1,in1 IN,in2 N){\
        return (IN op (short)N) __VA_ARGS__; \
    }

//移位运算
#define iec_move(iec_type)\
    iec_trans(SHL,IE##iec_type, IEC_SINT, << )\
    iec_trans(SHR,IE##iec_type, IEC_SINT, >> )\
    iec_trans(ROR,IE##iec_type, IEC_SINT, >> , | (IN << ((short)(8 * sizeof(IE##iec_type) - (N % (8 * sizeof(IE##iec_type)))))))\
    iec_trans(ROL,IE##iec_type, IEC_SINT, << , | (IN >> ((short)(8 * sizeof(IE##iec_type) - (N % (8 * sizeof(IE##iec_type)))))))\
    iec_trans(SHL,IE##iec_type, IEC_INT, << )\
    iec_trans(SHR,IE##iec_type, IEC_INT, >> )\
    iec_trans(ROR,IE##iec_type, IEC_INT, >> , | (IN << ((short)(8 * sizeof(IE##iec_type) - (N % (8 * sizeof(IE##iec_type)))))))\
    iec_trans(ROL,IE##iec_type, IEC_INT, << , | (IN >> ((short)(8 * sizeof(IE##iec_type) - (N % (8 * sizeof(IE##iec_type)))))))

iec_move(C_BYTE)
iec_move(C_WORD)
iec_move(C_DWORD)

//重定位调用

#define SHL__IEC_BYTE__IEC_BYTE__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHL__IEC_BYTE__IEC_BYTE__IEC_SINT)+a))(p0,p1,IN,N)\

#define SHR__IEC_BYTE__IEC_BYTE__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHR__IEC_BYTE__IEC_BYTE__IEC_SINT)+a))(p0,p1,IN,N)\

#define ROR__IEC_BYTE__IEC_BYTE__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROR__IEC_BYTE__IEC_BYTE__IEC_SINT)+a))(p0,p1,IN,N)\

#define ROL__IEC_BYTE__IEC_BYTE__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROL__IEC_BYTE__IEC_BYTE__IEC_SINT)+a))(p0,p1,IN,N)\

#define SHL__IEC_BYTE__IEC_BYTE__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHL__IEC_BYTE__IEC_BYTE__IEC_INT)+a))(p0,p1,IN,N)\

#define SHR__IEC_BYTE__IEC_BYTE__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHR__IEC_BYTE__IEC_BYTE__IEC_INT)+a))(p0,p1,IN,N)\

#define ROR__IEC_BYTE__IEC_BYTE__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROR__IEC_BYTE__IEC_BYTE__IEC_INT)+a))(p0,p1,IN,N)\

#define ROL__IEC_BYTE__IEC_BYTE__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROL__IEC_BYTE__IEC_BYTE__IEC_INT)+a))(p0,p1,IN,N)\

//iec_word

#define SHL__IEC_WORD__IEC_WORD__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHL__IEC_WORD__IEC_WORD__IEC_SINT)+a))(p0,p1,IN,N)\

#define SHR__IEC_WORD__IEC_WORD__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHR__IEC_WORD__IEC_WORD__IEC_SINT)+a))(p0,p1,IN,N)\

#define ROR__IEC_WORD__IEC_WORD__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROR__IEC_WORD__IEC_WORD__IEC_SINT)+a))(p0,p1,IN,N)\

#define ROL__IEC_WORD__IEC_WORD__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROL__IEC_WORD__IEC_WORD__IEC_SINT)+a))(p0,p1,IN,N)\

#define SHL__IEC_WORD__IEC_WORD__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHL__IEC_WORD__IEC_WORD__IEC_INT)+a))(p0,p1,IN,N)\

#define SHR__IEC_WORD__IEC_WORD__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHR__IEC_WORD__IEC_WORD__IEC_INT)+a))(p0,p1,IN,N)\

#define ROR__IEC_WORD__IEC_WORD__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROR__IEC_WORD__IEC_WORD__IEC_INT)+a))(p0,p1,IN,N)\

#define ROL__IEC_WORD__IEC_WORD__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROL__IEC_WORD__IEC_WORD__IEC_INT)+a))(p0,p1,IN,N)\

//iec_dword

#define SHL__IEC_DWORD__IEC_DWORD__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHL__IEC_DWORD__IEC_DWORD__IEC_SINT)+a))(p0,p1,IN,N)\

#define SHR__IEC_DWORD__IEC_DWORD__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHR__IEC_DWORD__IEC_DWORD__IEC_SINT)+a))(p0,p1,IN,N)\

#define ROR__IEC_DWORD__IEC_DWORD__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROR__IEC_DWORD__IEC_DWORD__IEC_SINT)+a))(p0,p1,IN,N)\

#define ROL__IEC_DWORD__IEC_DWORD__IEC_SINT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROL__IEC_DWORD__IEC_DWORD__IEC_SINT)+a))(p0,p1,IN,N)\

#define SHL__IEC_DWORD__IEC_DWORD__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHL__IEC_DWORD__IEC_DWORD__IEC_INT)+a))(p0,p1,IN,N)\

#define SHR__IEC_DWORD__IEC_DWORD__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_SHR__IEC_DWORD__IEC_DWORD__IEC_INT)+a))(p0,p1,IN,N)\

#define ROR__IEC_DWORD__IEC_DWORD__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROR__IEC_DWORD__IEC_DWORD__IEC_INT)+a))(p0,p1,IN,N)\

#define ROL__IEC_DWORD__IEC_DWORD__IEC_INT(p0,p1,IN,N)\
    ((ptrans)((ll)(ins_ROL__IEC_DWORD__IEC_DWORD__IEC_INT)+a))(p0,p1,IN,N)\
//END
iec_t __time_to_timespec(int flg, int day, int hour, int mi, int sec, int ms) {
    iec_t ret;
    if (flg) {
        ret.microsec = 0;
        ret.microsec = (((day * 24 + hour) * 60 + mi) * 60 + sec) * 1000;
    }
}

iec_t __date_to_timespec(int day, int month, int year) {
    iec_t ret;
    ret.microsec = 0;
    ret.microsec = (((day * 24 + 0) * 60 + 0) * 60 + 0) * 1000;

}

#define RelocPoint(type,var)\
    (*((type*)((ll)(&(var)) + a)))

#define __SET_VAR(type,prefix, name, suffix, new_value)\
	*((type*)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)&(RelocPoint(type,prefix name suffix))))) = new_value

#define __SET_EXTERNAL(type,prefix, name, suffix, new_value)\
    *((type*)RelocPointF(Handle, handle)((*((unsigned int*)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)&(RelocPoint(type*,prefix name)))))))) = new_value

#define __SET_EXTERNALS(exttype,type,prefix, name, suffix, new_value)\
    (*((exttype*)RelocPointF(Handle, handle)((*((unsigned int*)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)&(RelocPoint(type*,prefix name)))))))))suffix = new_value
//*(*((type**)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)&(RelocPoint(type*,prefix name suffix)))))) = new_value

#define __GET_VAR(type,name,acc,...)\
	*((type*)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)(&RelocPoint(type,name acc))))) __VA_ARGS__

#define __GET_EXTERNALS(exttype,type,name,acc,...)\
     (*((exttype*)RelocPointF(Handle, handle)((*((unsigned int*)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)&(RelocPoint(type*,name)))))))))acc

#define __GET_EXTERNAL(type,name,acc,...)\
        *((type*)RelocPointF(Handle, handle)((*((unsigned int*)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)&(RelocPoint(type*,name))))))))
    //*(*((type**)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)(&RelocPoint(type*,name acc)))))) __VA_ARGS__

#define __INIT_VAR(name, initial, retained,type)\
	*((type*)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)&RelocPoint(type,name)))) = initial;

#define __INIT_EXTERNAL(type, es, P, T)\
	*((type**)RelocPointF(Handle, handle)(RelocPointF(Find, find)((ll)&RelocPoint(type*,P)))) = (unsigned int)(RelocPointF(Find,find)((ll)&RelocPoint(type,es)));//

#define __DECLARE_GLOBAL(ID,type,dummy, name)\
	type name __attribute__((__section__(".bss")));

#define __DECLARE_VAR(type, name)\
	type name;

#define __DECLARE_EXTERNAL(type, name)\
	type * name;

#define LOCATED_VAR(P,type,T,UID)\
	RelocPointF(Insert, insert)(T,(ll)(&(RelocPoint(type,P))),UID);

#define GLOBAL_INSERT(type,es,P,T,UID)\
	RelocPointF(Insert, insert)(T,(ll)(&(RelocPoint(type*,P))),UID);


#define __DECLARE_STRUCT_TYPE(type, elements)\
typedef struct {\
  elements\
} type;\

#define __DECLARE_ARRAY_TYPE(type, base, size)\
typedef struct {\
  base table [size];\
} type;\
void __##type##_register( type * ins,unsigned char * a,int uid){\
	for(int i=0;i<size;i++){\
		LOCATED_VAR(ins->table[i],base,0,uid);\
		uid++;\
    }\
}\
typedef void(* p__##type##_register)(type * ins,unsigned char * a,int uid);\

#define RELLOREG_VAR(P,type,T,UID)\
	RelocPointF(Insert, insert)(T,(ll)(&(RelocPoint(type,P))),UID);

#define DirectInsert(UID,domain,offset,step,len)\
    RelocPointF(DInsert, dinsert)(UID, domain, offset, step, len);
//DirectInsert(30889, 'M', ofs += sizeof(IEC_BYTE, 0, 0, 0), 'B', sizeof(IEC_BYTE, 0, 0, 0));//SYS_COUNTER10


/*
#undef extIEC_INT 3
#undef extIEC_UINT 3
#undef extIEC_DINT 0
#undef extIEC_UDINT 0
#undef extIEC_BOOL 1
#undef extIEC_WORD 1
#undef extIEC_REAL 2
*/
//#undef extIEC_INT 0
//#undef extIEC_DINT 0
//#undef extIEC_BOOL 1
//#undef extIEC_CHAR 1
//#undef extIEC_FLOAT 2