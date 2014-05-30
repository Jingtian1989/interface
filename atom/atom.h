#ifndef __ATOM_H__
#define __ATOM_H__

extern 	int  		atom_length	(const char *char);
extern 	const char*	atom_new	(const char *str, int len);
extern 	const char* atom_string (const char *str);
extern 	const char* atom_int	(long n);
extern 	void 		atom_free	(const char *str);
extern  void 		atom_reset	(void);
extern 	void 		atom_vload 	(const char *str, ...);
extern	void 		atom_aload	(const char *strs[]);
extern  void 		atom_add 	(const char *str, int len);

#endif