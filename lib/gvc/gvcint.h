/* $Id$ $Revision$ */
/* vim:set shiftwidth=4 ts=8: */

/**********************************************************
*      This software is part of the graphviz package      *
*                http://www.graphviz.org/                 *
*                                                         *
*            Copyright (c) 1994-2004 AT&T Corp.           *
*                and is licensed under the                *
*            Common Public License, Version 1.0           *
*                      by AT&T Corp.                      *
*                                                         *
*        Information and Software Systems Research        *
*              AT&T Research, Florham Park NJ             *
**********************************************************/

/* Common header used by both clients and plugins */

#ifndef GVCINT_H
#define GVCINT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE (! FALSE)
#endif

#define ARRAY_SIZE(A) (sizeof(A)/sizeof(A[0]))

    typedef enum { PEN_NONE, PEN_DASHED, PEN_DOTTED, PEN_SOLID } pen_type;
    typedef enum { FILL_NONE, FILL_SOLID } fill_type;
    typedef enum { FONT_REGULAR, FONT_BOLD, FONT_ITALIC } font_type;
#define PENWIDTH_NORMAL 1.
#define PENWIDTH_BOLD 2.

    typedef struct {
	char *fontfam, fontopt;
	color_t pencolor, fillcolor;
	pen_type pen;
	fill_type fill;
	double penwidth;
	double fontsz;
    } gvstyle_t;

#define EMIT_SORTED (1<<0)
#define EMIT_COLORS (1<<1)
#define EMIT_CLUSTERS_LAST (1<<2)
#define EMIT_PREORDER (1<<3)
#define EMIT_EDGE_SORTED (1<<4)
#define GVRENDER_DOES_ARROWS (1<<5)
#define GVRENDER_DOES_LAYERS (1<<6)
#define GVRENDER_DOES_MULTIGRAPH_OUTPUT_FILES (1<<7)
#define GVRENDER_DOES_TRUECOLOR (1<<8)
#define GVRENDER_Y_GOES_DOWN (1<<9)
#define GVRENDER_X11_EVENTS (1<<10)
#define GVRENDER_DOES_TRANSFORM (1<<11)

    typedef struct {
	int flags;
	double default_margin; /* graph units */
	int default_dpi;
	char **knowncolors;
	int sz_knowncolors;
	color_type_t color_type;
	char *device;
    } gvrender_features_t;

    typedef struct {
	int flags;
    } gvdevice_features_t;

#define LAYOUT_USES_RANKDIR (1<<0)

    typedef struct {
	int flags;
    } gvlayout_features_t;

    /* active plugin headers */
    typedef struct gvplugin_active_device_s {
        gvdevice_engine_t *engine;
        int id;
        gvdevice_features_t *features;
    } gvplugin_active_device_t;

    typedef struct gvplugin_active_layout_s {
        gvlayout_engine_t *engine;
        int id;
        gvlayout_features_t *features;
        char *type;
    } gvplugin_active_layout_t;

    typedef struct gvplugin_active_render_s {
        gvrender_engine_t *engine;
        int id;
        gvrender_features_t *features;
        char *type;
    } gvplugin_active_render_t;

    typedef struct gvplugin_active_textlayout_s {
        gvtextlayout_engine_t *engine;
        int id;
        char *type;
    } gvplugin_active_textlayout_t;

    typedef struct gvplugin_active_usershape_s {
        gvusershape_engine_t *engine;
        int id;
        char *type;
    } gvplugin_active_usershape_t;

    /*
     * gv_matrix_t:   (compat with cairo_matrix_t)
     *
     * A #gv_matrix_t holds an affine transformation, such as a scale,
     * rotation, or shear, or a combination of those.
     */
    typedef struct gv_matrix_s {
	double xx; double yx;
	double xy; double yy;
	double x0; double y0;
    } gv_matrix_t;

    typedef struct gv_argvlist_s {
	char **argv;
	int argc;
	int alloc;
    } gv_argvlist_t;

    typedef struct gvdevice_callbacks_s {
	void (*refresh) (GVJ_t * job);
        void (*button_press) (GVJ_t * job, int button, pointf pointer);
        void (*button_release) (GVJ_t * job, int button, pointf pointer);
        void (*motion) (GVJ_t * job, pointf pointer);
        void (*modify) (GVJ_t * job, char *name, char *value);
    } gvdevice_callbacks_t;

    struct GVJ_s {
	GVJ_t *next;  /* linked list of jobs */
	GVJ_t *next_active;   /* linked list of active jobs (e.g. multiple windows) */
	char *output_filename;
	char *output_langname;
	FILE *output_file;
	int output_lang;

	gvplugin_active_render_t render;
	gvplugin_active_device_t device;
	gvdevice_callbacks_t *callbacks;

#ifndef DISABLE_CODEGENS
	codegen_t *codegen;	/* current  codegen */
#endif

	void *surface;		/* gd or cairo surface */
	boolean external_surface; /* surface belongs to caller */

	gvstyle_t *style;       /* active style from gvc->styles[] */

	GVC_t *gvc;		/* parent gvc */
        graph_t *g;		/* parent graph */

	/* current graph object during rendering 
	 - only valid between gvrender_begin_xxx() and gvrender_end_xxx() */
        graph_t *sg;		/* current graph or subgraph/cluster */
        node_t *n;		/* current node */
        edge_t *e;		/* current edge */

        int flags;		/* emit_graph flags */

        pointf margin;		 /* job-specific margin - graph units */

	int numLayers;		/* number of layers */
	int layerNum;		/* current layer - 1 based*/

	boxf	pageBox;	 /* current page in graph coords */
	pointf	pageOffset;	 /* offset for current page in graph coords */
	pointf	pageSize;	 /* page size in graph units */
	point 	pagesArraySize;  /* 2D size of page array */
	point	pagesArrayFirst; /* 2D starting corner in */
	point	pagesArrayMajor; /* 2D major increment */
	point	pagesArrayMinor; /* 2D minor increment */
	point	pagesArrayElem;  /* 2D coord of current page - 0,0 based */
        int	numPages;	 /* number of pages */

        unsigned int width;     /* device width in device units */
        unsigned int height;    /* device height in device units */
	int dpi;		/* device resolution device-units-per-inch */
	double zoom;		/* viewport zoom factor */
	int rotation;		/* viewport rotation  0=portrait, 1=landscape */
	pointf focus;		/* viewport focus in graph units */
	box	boundingBox;	/* drawable region in device units */

	boxf clip;		/* clip region in graph units */
	boxf pageBoxClip;       /* intersection of clip and pageBox */

	gv_matrix_t transform;  /* transformation matrix for renderers that can use it */
	pointf compscale;	/* composite device scale incl: scale, zoom, dpi, y_goes_down */
	pointf offset;		/* composite translation */
	
	boolean fit_mode,
		needs_refresh,
		click,
		active,
		has_grown;

	pointf pointer;		/* pointer position in device units */
	pointf oldpointer;	/* old pointer position in device units */

	void *current_obj;      /* graph object that pointer is in currently */

	void *selected_obj;      /* graph object that has been selected */
					/* (e.g. button 1 clicked on current obj) */
	gv_argvlist_t selected_obj_type_name; /* (e.g. "edge" "node3" "e" "->" "node5" "") */
	gv_argvlist_t selected_obj_attributes; /* even args are names, odd are values */
				/* e.g. "color" "red" "style" "filled" */

	void *window;		/* display-specific data for gvrender plugin */
    };

    typedef struct gvplugin_available_s gvplugin_available_t;

    struct gvplugin_available_s {
	gvplugin_available_t *next;       /* next plugin in linked list, or NULL */
	char *typestr;		 /* type string, e.g. "png" or "ps" */
	int quality;             /* programmer assigned quality
					ranking within type (+ve or -ve int)
					- codegens have quality = 0 */
	char *path;		 /* file path to library containing plugin,
					or NULL if builtin */
	char *packagename;	 /* package name */
	gvplugin_installed_t *typeptr;  /* pointer to jumptable for plugin,
					or NULL if not yet loaded */
    };

    typedef int (*gvevent_key_callback_t) (GVJ_t * job);

    typedef struct gvevent_key_binding_s {
	char *keystring;
	gvevent_key_callback_t callback;
    } gvevent_key_binding_t;

#define MAXNEST 4

    struct GVC_s {
	/* gvNEWcontext() */
	char *user;
	char **info;

	/* gvrender_config() */
	GVJ_t *jobs;	/* linked list of jobs */
	GVJ_t *job;	/* current job */
	void (*errorfn) (char *fmt, ...);

	/* plugins */
#define ELEM(x) +1
	/* APIS expands to "+1 +1 ... +1" to give the number of APIs */
	gvplugin_available_t *apis[ APIS ]; /* array of linked-list of plugins per api */
	gvplugin_available_t *api[ APIS ];  /* array of current plugins per api */
#undef ELEM

	/* gvrender_begin_job() */
	gvplugin_active_textlayout_t textlayout;
	gvplugin_active_usershape_t usershape;
	gvplugin_active_layout_t layout;

	char *graphname;	/* name from graph */
	GVJ_t *active_jobs;   /* linked list of active jobs */

	char **lib;

	/* pagination */
	pointf margin;		/* margins in graph units */
	pointf pageSize;	/* pageSize in graph units, not including margins */
	point pb;		/* page size - including margins (inches) */
	boxf bb;		/* graph bb in graph units, not including margins */
	int rotation;		/* rotation - 0 = portrait, 90 = landscape */
	boolean graph_sets_margin, graph_sets_pageSize, graph_sets_rotation;

	/* layers */
	char *layerDelims;	/* delimiters in layer names */
	char *layers;		/* null delimited list of layer names */
	char **layerIDs;	/* array of layer names */
	int numLayers;		/* number of layers */

        int viewNum;	        /* current view - 1 based count of views,
					all pages in all layers */
	/* default font */
	char *defaultfontname;
	double defaultfontsize;

	/* default line style */
	char **defaultlinestyle;

	gvstyle_t styles[MAXNEST]; /* style stack - reused by each job */
	int SP;

	/* render defaults set from graph */
	color_t bgcolor;	/* background color */

	/* keybindings for keyboard events */
	gvevent_key_binding_t *keybindings;
	int numkeys;
	void *keycodes;
    };

#ifdef __cplusplus
}
#endif
#endif				/* GVCINT_H */
