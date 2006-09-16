/**
 * \file libyasm/preproc.h
 * \brief YASM preprocessor module interface.
 *
 * \rcs
 * $Id$
 * \endrcs
 *
 * \license
 *  Copyright (C) 2001  Peter Johnson
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND OTHER CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR OTHER CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * \endlicense
 */
#ifndef YASM_PREPROC_H
#define YASM_PREPROC_H

#ifndef YASM_DOXYGEN
/** Base #yasm_preproc structure.  Must be present as the first element in any
 * #yasm_preproc implementation.
 */
typedef struct yasm_preproc_base {
    /** #yasm_preproc_module implementation for this preprocessor. */
    const struct yasm_preproc_module *module;
} yasm_preproc_base;
#endif

/** YASM preprocesor module interface. */
typedef struct yasm_preproc_module {
    /** One-line description of the preprocessor. */
    const char *name;

    /** Keyword used to select preprocessor on the command line. */
    const char *keyword;

    /** Create preprocessor.
     * Module-level implementation of yasm_preproc_create().
     * Call yasm_preproc_create() instead of calling this function.
     *
     * \param f			initial starting file
     * \param in_filename	initial starting filename
     * \param lm		line mapping repository
     * \param errwarns		error/warnning set.
     * \return New preprocessor.
     *
     * \note Any preprocessor errors and warnings are stored into errwarns.
     */
    /*@only@*/ yasm_preproc * (*create) (FILE *f, const char *in_filename,
					 yasm_linemap *lm,
					 yasm_errwarns *errwarns);

    /** Module-level implementation of yasm_preproc_destroy().
     * Call yasm_preproc_destroy() instead of calling this function.
     */
    void (*destroy) (/*@only@*/ yasm_preproc *preproc);

    /** Module-level implementation of yasm_preproc_input().
     * Call yasm_preproc_input() instead of calling this function.
     */
    size_t (*input) (yasm_preproc *preproc, /*@out@*/ char *buf,
		     size_t max_size);

    /** Module-level implementation of yasm_preproc_get_included_file().
     * Call yasm_preproc_get_included_file() instead of calling this function.
     */
    size_t (*get_included_file) (yasm_preproc *preproc, /*@out@*/ char *buf,
                                 size_t max_size);

    /** Module-level implementation of yasm_preproc_add_include_path().
     * Call yasm_preproc_add_include_path() instead of calling this function.
     */
    void (*add_include_path) (yasm_preproc *preproc, const char *path);

    /** Module-level implementation of yasm_preproc_add_include_file().
     * Call yasm_preproc_add_include_file() instead of calling this function.
     */
    void (*add_include_file) (yasm_preproc *preproc, const char *filename);

    /** Module-level implementation of yasm_preproc_predefine_macro().
     * Call yasm_preproc_predefine_macro() instead of calling this function.
     */
    void (*predefine_macro) (yasm_preproc *preproc, const char *macronameval);

    /** Module-level implementation of yasm_preproc_undefine_macro().
     * Call yasm_preproc_undefine_macro() instead of calling this function.
     */
    void (*undefine_macro) (yasm_preproc *preproc, const char *macroname);

    /** Module-level implementation of yasm_preproc_builtin_define().
     * Call yasm_preproc_builtin_define() instead of calling this function.
     */
    void (*define_builtin) (yasm_preproc *preproc, const char *macronameval);
} yasm_preproc_module;

/** Initialize preprocessor.
 * The preprocessor needs access to the object format module to find out
 * any output format specific macros.
 * \param module	preprocessor module
 * \param f		initial starting file
 * \param in_filename	initial starting file filename
 * \param lm		line mapping repository
 * \param errwarns	error/warning set
 * \return New preprocessor.
 * \note Errors/warnings are stored into errwarns.
 */
/*@only@*/ yasm_preproc *yasm_preproc_create
    (yasm_preproc_module *module, FILE *f, const char *in_filename,
     yasm_linemap *lm, yasm_errwarns *errwarns);

/** Cleans up any allocated preproc memory.
 * \param preproc	preprocessor
 */
void yasm_preproc_destroy(/*@only@*/ yasm_preproc *preproc);

/** Gets more preprocessed source code (up to max_size bytes) into buf.
 * More than a single line may be returned in buf.
 * \param preproc	preprocessor
 * \param buf		destination buffer for preprocessed source
 * \param max_size	maximum number of bytes that can be returned in buf
 * \return Actual number of bytes returned in buf.
 */
size_t yasm_preproc_input(yasm_preproc *preproc, /*@out@*/ char *buf,
			  size_t max_size);

/** Get the next filename included by the source code.
 * \param preproc	preprocessor
 * \param buf		destination buffer for filename
 * \param max_size	maximum number of bytes that can be returned in buf
 * \return Actual number of bytes returned in buf.
 */
size_t yasm_preproc_get_included_file(yasm_preproc *preproc,
				      /*@out@*/ char *buf, size_t max_size);

/** Add a directory to the include search path.
 * \param preproc	preprocessor
 * \param path		pathname
 */
void yasm_preproc_add_include_path(yasm_preproc *preproc, const char *path);

/** Pre-include a file.
 * \param preproc	preprocessor
 * \param filename	filename
 */
void yasm_preproc_add_include_file(yasm_preproc *preproc,
				   const char *filename);

/** Pre-define a macro.
 * \param preproc	preprocessor
 * \param macronameval	"name=value" string
 */
void yasm_preproc_predefine_macro(yasm_preproc *preproc,
				  const char *macronameval);

/** Un-define a macro.
 * \param preproc	preprocessor
 * \param macroname	macro name
 */
void yasm_preproc_undefine_macro(yasm_preproc *preproc, const char *macroname);

/** Define a builtin macro, preprocessed before the "standard" macros.
 * \param preproc	preprocessor
 * \param macronameval	"name=value" string
 */
void yasm_preproc_define_builtin(yasm_preproc *preproc,
				 const char *macronameval);

#ifndef YASM_DOXYGEN

/* Inline macro implementations for preproc functions */

#define yasm_preproc_create(module, f, in_filename, lm, ews) \
    module->create(f, in_filename, lm, ews)

#define yasm_preproc_destroy(preproc) \
    ((yasm_preproc_base *)preproc)->module->destroy(preproc)
#define yasm_preproc_input(preproc, buf, max_size) \
    ((yasm_preproc_base *)preproc)->module->input(preproc, buf, max_size)
#define yasm_preproc_get_included_file(preproc, buf, max_size) \
    ((yasm_preproc_base *)preproc)->module->get_included_file(preproc, buf, max_size)
#define yasm_preproc_add_include_path(preproc, path) \
    ((yasm_preproc_base *)preproc)->module->add_include_path(preproc, path)
#define yasm_preproc_add_include_file(preproc, filename) \
    ((yasm_preproc_base *)preproc)->module->add_include_file(preproc, filename)
#define yasm_preproc_predefine_macro(preproc, macronameval) \
    ((yasm_preproc_base *)preproc)->module->predefine_macro(preproc, \
							    macronameval)
#define yasm_preproc_undefine_macro(preproc, macroname) \
    ((yasm_preproc_base *)preproc)->module->undefine_macro(preproc, macroname)
#define yasm_preproc_define_builtin(preproc, macronameval) \
    ((yasm_preproc_base *)preproc)->module->define_builtin(preproc, \
							   macronameval)

#endif

#endif
