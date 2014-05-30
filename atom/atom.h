#ifndef __ATOM_H__
#define __ATOM_H__

extern 	int  		atom_length	(const char *char);
extern 	const char*	atom_new	(const char *str, int len);
extern 	const char* atom_string (const char *str);
extern 	const char* atom_int	(long n);

#endif