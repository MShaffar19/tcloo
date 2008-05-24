/*
 * tclOOBasic.c --
 *
 *	This file contains implementations of the "simple" commands and
 *	methods from the object-system core.
 *
 * Copyright (c) 2005-2008 by Donal K. Fellows
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id: tclOOBasic.c,v 1.1 2008/05/24 08:08:26 dkf Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "tclInt.h"
#include "tclOOInt.h"

/*
 * ----------------------------------------------------------------------
 *
 * TclOO_Class_Create --
 *
 *	Implementation for oo::class->create method.
 *
 * ----------------------------------------------------------------------
 */

int
TclOO_Class_Create(
    ClientData clientData,	/* Ignored. */
    Tcl_Interp *interp,		/* Interpreter in which to create the object;
				 * also used for error reporting. */
    Tcl_ObjectContext context,	/* The object/call context. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const *objv)	/* The actual arguments. */
{
    Object *oPtr = (Object *) Tcl_ObjectContextObject(context);
    Tcl_Object newObject;
    const char *objName;
    int len;

    /*
     * Sanity check; should not be possible to invoke this method on a
     * non-class.
     */

    if (oPtr->classPtr == NULL) {
	Tcl_Obj *cmdnameObj = TclOOObjectName(interp, oPtr);

	Tcl_AppendResult(interp, "object \"", TclGetString(cmdnameObj),
		"\" is not a class", NULL);
	return TCL_ERROR;
    }

    /*
     * Check we have the right number of (sensible) arguments.
     */

    if (objc - Tcl_ObjectContextSkippedArgs(context) < 1) {
	Tcl_WrongNumArgs(interp, Tcl_ObjectContextSkippedArgs(context), objv,
		"objectName ?arg ...?");
	return TCL_ERROR;
    }
    objName = Tcl_GetStringFromObj(
	    objv[Tcl_ObjectContextSkippedArgs(context)], &len);
    if (len == 0) {
	Tcl_AppendResult(interp, "object name must not be empty", NULL);
	return TCL_ERROR;
    }

    /*
     * Make the object and return its name.
     */

    newObject = Tcl_NewObjectInstance(interp, (Tcl_Class) oPtr->classPtr,
	    objName, NULL, objc, objv,
	    Tcl_ObjectContextSkippedArgs(context)+1);
    if (newObject == NULL) {
	return TCL_ERROR;
    }
    Tcl_SetObjResult(interp, TclOOObjectName(interp, (Object *) newObject));
    return TCL_OK;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOO_Class_CreateNs --
 *
 *	Implementation for oo::class->createWithNamespace method.
 *
 * ----------------------------------------------------------------------
 */

int
TclOO_Class_CreateNs(
    ClientData clientData,	/* Ignored. */
    Tcl_Interp *interp,		/* Interpreter in which to create the object;
				 * also used for error reporting. */
    Tcl_ObjectContext context,	/* The object/call context. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const *objv)	/* The actual arguments. */
{
    Object *oPtr = (Object *) Tcl_ObjectContextObject(context);
    Tcl_Object newObject;
    const char *objName, *nsName;
    int len;

    /*
     * Sanity check; should not be possible to invoke this method on a
     * non-class.
     */

    if (oPtr->classPtr == NULL) {
	Tcl_Obj *cmdnameObj = TclOOObjectName(interp, oPtr);

	Tcl_AppendResult(interp, "object \"", TclGetString(cmdnameObj),
		"\" is not a class", NULL);
	return TCL_ERROR;
    }

    /*
     * Check we have the right number of (sensible) arguments.
     */

    if (objc - Tcl_ObjectContextSkippedArgs(context) < 2) {
	Tcl_WrongNumArgs(interp, Tcl_ObjectContextSkippedArgs(context), objv,
		"objectName namespaceName ?arg ...?");
	return TCL_ERROR;
    }
    objName = Tcl_GetStringFromObj(
	    objv[Tcl_ObjectContextSkippedArgs(context)], &len);
    if (len == 0) {
	Tcl_AppendResult(interp, "object name must not be empty", NULL);
	return TCL_ERROR;
    }
    nsName = Tcl_GetStringFromObj(
	    objv[Tcl_ObjectContextSkippedArgs(context)+1], &len);
    if (len == 0) {
	Tcl_AppendResult(interp, "namespace name must not be empty", NULL);
	return TCL_ERROR;
    }

    /*
     * Make the object and return its name.
     */

    newObject = Tcl_NewObjectInstance(interp, (Tcl_Class) oPtr->classPtr,
	    objName, nsName, objc, objv,
	    Tcl_ObjectContextSkippedArgs(context)+2);
    if (newObject == NULL) {
	return TCL_ERROR;
    }
    Tcl_SetObjResult(interp, TclOOObjectName(interp, (Object *) newObject));
    return TCL_OK;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOO_Class_New --
 *
 *	Implementation for oo::class->new method.
 *
 * ----------------------------------------------------------------------
 */

int
TclOO_Class_New(
    ClientData clientData,	/* Ignored. */
    Tcl_Interp *interp,		/* Interpreter in which to create the object;
				 * also used for error reporting. */
    Tcl_ObjectContext context,	/* The object/call context. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const *objv)	/* The actual arguments. */
{
    Object *oPtr = (Object *) Tcl_ObjectContextObject(context);
    Tcl_Object newObject;

    /*
     * Sanity check; should not be possible to invoke this method on a
     * non-class.
     */

    if (oPtr->classPtr == NULL) {
	Tcl_Obj *cmdnameObj = TclOOObjectName(interp, oPtr);

	Tcl_AppendResult(interp, "object \"", TclGetString(cmdnameObj),
		"\" is not a class", NULL);
	return TCL_ERROR;
    }

    /*
     * Make the object and return its name.
     */

    newObject = Tcl_NewObjectInstance(interp, (Tcl_Class) oPtr->classPtr,
	    NULL, NULL, objc, objv, Tcl_ObjectContextSkippedArgs(context));
    if (newObject == NULL) {
	return TCL_ERROR;
    }
    Tcl_SetObjResult(interp, TclOOObjectName(interp, (Object *) newObject));
    return TCL_OK;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOO_Object_Destroy --
 *
 *	Implementation for oo::object->destroy method.
 *
 * ----------------------------------------------------------------------
 */

int
TclOO_Object_Destroy(
    ClientData clientData,	/* Ignored. */
    Tcl_Interp *interp,		/* Interpreter in which to create the object;
				 * also used for error reporting. */
    Tcl_ObjectContext context,	/* The object/call context. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const *objv)	/* The actual arguments. */
{
    if (objc != Tcl_ObjectContextSkippedArgs(context)) {
	Tcl_WrongNumArgs(interp, Tcl_ObjectContextSkippedArgs(context), objv,
		NULL);
	return TCL_ERROR;
    }
    Tcl_DeleteCommandFromToken(interp,
	    Tcl_GetObjectCommand(Tcl_ObjectContextObject(context)));
    return TCL_OK;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOO_Object_Eval --
 *
 *	Implementation for oo::object->eval method.
 *
 * ----------------------------------------------------------------------
 */

int
TclOO_Object_Eval(
    ClientData clientData,	/* Ignored. */
    Tcl_Interp *interp,		/* Interpreter in which to create the object;
				 * also used for error reporting. */
    Tcl_ObjectContext context,	/* The object/call context. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const *objv)	/* The actual arguments. */
{
    CallContext *contextPtr = (CallContext *) context;
    Tcl_Object object = Tcl_ObjectContextObject(context);
    CallFrame *framePtr, **framePtrPtr;
    Tcl_Obj *objnameObj;
    int result;

    if (objc-1 < Tcl_ObjectContextSkippedArgs(context)) {
	Tcl_WrongNumArgs(interp, Tcl_ObjectContextSkippedArgs(context), objv,
		"arg ?arg ...?");
	return TCL_ERROR;
    }

    /*
     * Make the object's namespace the current namespace and evaluate the
     * command(s).
     */

    /* This is needed to satisfy GCC 3.3's strict aliasing rules */
    framePtrPtr = &framePtr;
    result = TclPushStackFrame(interp, (Tcl_CallFrame **) framePtrPtr,
	    Tcl_GetObjectNamespace(object), 0);
    if (result != TCL_OK) {
	return TCL_ERROR;
    }
    framePtr->objc = objc;
    framePtr->objv = objv;	/* Reference counts do not need to be
				 * incremented here. */

    if (contextPtr->callPtr->flags & PUBLIC_METHOD) {
	objnameObj = TclOOObjectName(interp, (Object *) object);
    } else {
	objnameObj = Tcl_NewStringObj("my", 2);
    }
    Tcl_IncrRefCount(objnameObj);

    if (objc == Tcl_ObjectContextSkippedArgs(context)+1) {
	result = Tcl_EvalObjEx(interp,
		objv[Tcl_ObjectContextSkippedArgs(context)], 0);
    } else {
	Tcl_Obj *objPtr;

	/*
	 * More than one argument: concatenate them together with spaces
	 * between, then evaluate the result. Tcl_EvalObjEx will delete the
	 * object when it decrements its refcount after eval'ing it.
	 */

	objPtr = Tcl_ConcatObj(objc-Tcl_ObjectContextSkippedArgs(context),
		objv+Tcl_ObjectContextSkippedArgs(context));
	result = Tcl_EvalObjEx(interp, objPtr, TCL_EVAL_DIRECT);
    }

    if (result == TCL_ERROR) {
	Tcl_AppendObjToErrorInfo(interp, Tcl_ObjPrintf(
		"\n    (in \"%s eval\" script line %d)",
		TclGetString(objnameObj), interp->errorLine));
    }

    /*
     * Restore the previous "current" namespace.
     */

    TclPopStackFrame(interp);
    Tcl_DecrRefCount(objnameObj);
    return result;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOO_Object_Unknown --
 *
 *	Default unknown method handler method (defined in oo::object). This
 *	just creates a suitable error message.
 *
 * ----------------------------------------------------------------------
 */

int
TclOO_Object_Unknown(
    ClientData clientData,	/* Ignored. */
    Tcl_Interp *interp,		/* Interpreter in which to create the object;
				 * also used for error reporting. */
    Tcl_ObjectContext context,	/* The object/call context. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const *objv)	/* The actual arguments. */
{
    CallContext *contextPtr = (CallContext *) context;
    Object *oPtr = contextPtr->oPtr;
    const char **methodNames;
    int numMethodNames, i, skip = Tcl_ObjectContextSkippedArgs(context);

    if (objc < skip+1) {
	Tcl_WrongNumArgs(interp, skip, objv, "methodName ?arg ...?");
	return TCL_ERROR;
    }

    /*
     * Get the list of methods that we want to know about.
     */

    numMethodNames = TclOOGetSortedMethodList(oPtr,
	    contextPtr->callPtr->flags & PUBLIC_METHOD, &methodNames);

    /*
     * Special message when there are no visible methods at all.
     */

    if (numMethodNames == 0) {
	Tcl_Obj *tmpBuf = TclOOObjectName(interp, oPtr);

	Tcl_AppendResult(interp, "object \"", TclGetString(tmpBuf), NULL);
	if (contextPtr->callPtr->flags & PUBLIC_METHOD) {
	    Tcl_AppendResult(interp, "\" has no visible methods", NULL);
	} else {
	    Tcl_AppendResult(interp, "\" has no methods", NULL);
	}
	return TCL_ERROR;
    }

    Tcl_AppendResult(interp, "unknown method \"", TclGetString(objv[skip]),
	    "\": must be ", NULL);
    for (i=0 ; i<numMethodNames-1 ; i++) {
	if (i) {
	    Tcl_AppendResult(interp, ", ", NULL);
	}
	Tcl_AppendResult(interp, methodNames[i], NULL);
    }
    if (i) {
	Tcl_AppendResult(interp, " or ", NULL);
    }
    Tcl_AppendResult(interp, methodNames[i], NULL);
    ckfree((char *) methodNames);
    return TCL_ERROR;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOO_Object_LinkVar --
 *
 *	Implementation of oo::object->variable method.
 *
 * ----------------------------------------------------------------------
 */

int
TclOO_Object_LinkVar(
    ClientData clientData,	/* Ignored. */
    Tcl_Interp *interp,		/* Interpreter in which to create the object;
				 * also used for error reporting. */
    Tcl_ObjectContext context,	/* The object/call context. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const *objv)	/* The actual arguments. */
{
    Interp *iPtr = (Interp *) interp;
    Tcl_Object object = Tcl_ObjectContextObject(context);
    Namespace *savedNsPtr;
    int i;

    if (objc-Tcl_ObjectContextSkippedArgs(context) < 1) {
	Tcl_WrongNumArgs(interp, Tcl_ObjectContextSkippedArgs(context), objv,
		"varName ?varName ...?");
	return TCL_ERROR;
    }

    /*
     * Do nothing if we are not called from the body of a method. In this
     * respect, we are like the [global] command.
     */

    if (iPtr->varFramePtr == NULL ||
	    !(iPtr->varFramePtr->isProcCallFrame & FRAME_IS_METHOD)) {
	return TCL_OK;
    }

    for (i=Tcl_ObjectContextSkippedArgs(context) ; i<objc ; i++) {
	Var *varPtr, *aryPtr;
	const char *varName = TclGetString(objv[i]);

	/*
	 * The variable name must not contain a '::' since that's illegal in
	 * local names.
	 */

	if (strstr(varName, "::") != NULL) {
	    Tcl_AppendResult(interp, "variable name \"", varName,
		    "\" illegal: must not contain namespace separator", NULL);
	    return TCL_ERROR;
	}

	/*
	 * Switch to the object's namespace for the duration of this call.
	 * Like this, the variable is looked up in the namespace of the
	 * object, and not in the namespace of the caller. Otherwise this
	 * would only work if the caller was a method of the object itself,
	 * which might not be true if the method was exported. This is a bit
	 * of a hack, but the simplest way to do this (pushing a stack frame
	 * would be horribly expensive by comparison). We never have to worry
	 * about the case where we're dealing with the global namespace; we've
	 * already checked that we are inside a method.
	 */

	savedNsPtr = iPtr->varFramePtr->nsPtr;
	iPtr->varFramePtr->nsPtr = (Namespace *)
		Tcl_GetObjectNamespace(object);
	varPtr = TclObjLookupVar(interp, objv[i], NULL, TCL_NAMESPACE_ONLY,
		"define", 1, 0, &aryPtr);
	iPtr->varFramePtr->nsPtr = savedNsPtr;

	if (varPtr == NULL || aryPtr != NULL) {
	    /*
	     * Variable cannot be an element in an array. If aryPtr is not
	     * NULL, it is an element, so throw up an error and return.
	     */

	    TclVarErrMsg(interp, varName, NULL, "define",
		    "name refers to an element in an array");
	    return TCL_ERROR;
	}

	/*
	 * Arrange for the lifetime of the variable to be correctly managed.
	 * This is copied out of Tcl_VariableObjCmd...
	 */

	if (!TclIsVarNamespaceVar(varPtr)) {
	    TclSetVarNamespaceVar(varPtr);
	}

	if (TclPtrMakeUpvar(interp, varPtr, varName, 0, -1) != TCL_OK) {
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOO_Object_VarName --
 *
 *	Implementation of the oo::object->varname method.
 *
 * ----------------------------------------------------------------------
 */

int
TclOO_Object_VarName(
    ClientData clientData,	/* Ignored. */
    Tcl_Interp *interp,		/* Interpreter in which to create the object;
				 * also used for error reporting. */
    Tcl_ObjectContext context,	/* The object/call context. */
    int objc,			/* Number of arguments. */
    Tcl_Obj *const *objv)	/* The actual arguments. */
{
    Interp *iPtr = (Interp *) interp;
    Var *varPtr, *aryVar;
    Tcl_Obj *varNamePtr;

    if (Tcl_ObjectContextSkippedArgs(context)+1 != objc) {
	Tcl_WrongNumArgs(interp, Tcl_ObjectContextSkippedArgs(context), objv,
		"varName");
	return TCL_ERROR;
    }

    /*
     * Switch to the object's namespace for the duration of this call. Like
     * this, the variable is looked up in the namespace of the object, and not
     * in the namespace of the caller. Otherwise this would only work if the
     * caller was a method of the object itself, which might not be true if
     * the method was exported. This is a bit of a hack, but the simplest way
     * to do this (pushing a stack frame would be horribly expensive by
     * comparison, and is only done when we'd otherwise interfere with the
     * global namespace).
     */

    if (iPtr->varFramePtr == NULL) {
	Tcl_CallFrame *dummyFrame;

	TclPushStackFrame(interp, &dummyFrame,
		Tcl_GetObjectNamespace(Tcl_ObjectContextObject(context)),0);
	varPtr = TclObjLookupVar(interp, objv[objc-1], NULL,
		TCL_NAMESPACE_ONLY|TCL_LEAVE_ERR_MSG, "refer to",1,1,&aryVar);
	TclPopStackFrame(interp);
    } else {
	Namespace *savedNsPtr;

	savedNsPtr = iPtr->varFramePtr->nsPtr;
	iPtr->varFramePtr->nsPtr = (Namespace *)
		Tcl_GetObjectNamespace(Tcl_ObjectContextObject(context));
	varPtr = TclObjLookupVar(interp, objv[objc-1], NULL,
		TCL_NAMESPACE_ONLY|TCL_LEAVE_ERR_MSG, "refer to",1,1,&aryVar);
	iPtr->varFramePtr->nsPtr = savedNsPtr;
    }

    if (varPtr == NULL) {
	return TCL_ERROR;
    }

    varNamePtr = Tcl_NewObj();
    Tcl_GetVariableFullName(interp, (Tcl_Var) varPtr, varNamePtr);
    Tcl_SetObjResult(interp, varNamePtr);
    return TCL_OK;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOONextObjCmd --
 *
 *	Implementation of the [next] command. Note that this command is only
 *	ever to be used inside the body of a procedure-like method.
 *
 * ----------------------------------------------------------------------
 */

int
TclOONextObjCmd(
    ClientData clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *const *objv)
{
    Interp *iPtr = (Interp *) interp;
    CallFrame *framePtr = iPtr->varFramePtr;
    Tcl_ObjectContext context;
    int result;

    /*
     * Start with sanity checks on the calling context to make sure that we
     * are invoked from a suitable method context. If so, we can safely
     * retrieve the handle to the object call context.
     */

    if (framePtr == NULL || !(framePtr->isProcCallFrame & FRAME_IS_METHOD)) {
	Tcl_AppendResult(interp, TclGetString(objv[0]),
		" may only be called from inside a method", NULL);
	return TCL_ERROR;
    }
    context = framePtr->clientData;

    /*
     * Invoke the (advanced) method call context in the caller context. Note
     * that this is like [uplevel 1] and not [eval].
     */

    iPtr->varFramePtr = framePtr->callerVarPtr;
    result = Tcl_ObjectContextInvokeNext(interp, context, objc, objv, 1);
    iPtr->varFramePtr = framePtr;
    return result;
}

/*
 * ----------------------------------------------------------------------
 *
 * TclOOSelfObjCmd --
 *
 *	Implementation of the [self] command, which provides introspection of
 *	the call context.
 *
 * ----------------------------------------------------------------------
 */

int
TclOOSelfObjCmd(
    ClientData clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *const *objv)
{
    static const char *subcmds[] = {
	"caller", "class", "filter", "method", "namespace", "next", "object",
	"target", NULL
    };
    enum SelfCmds {
	SELF_CALLER, SELF_CLASS, SELF_FILTER, SELF_METHOD, SELF_NS, SELF_NEXT,
	SELF_OBJECT, SELF_TARGET
    };
    Interp *iPtr = (Interp *) interp;
    CallFrame *framePtr = iPtr->varFramePtr;
    CallContext *contextPtr;
    int index;

#define CurrentlyInvoked(contextPtr) \
    ((contextPtr)->callPtr->chain[(contextPtr)->index])

    /*
     * Start with sanity checks on the calling context and the method context.
     */

    if (framePtr == NULL || !(framePtr->isProcCallFrame & FRAME_IS_METHOD)) {
	Tcl_AppendResult(interp, TclGetString(objv[0]),
		" may only be called from inside a method", NULL);
	return TCL_ERROR;
    }

    contextPtr = framePtr->clientData;

    /*
     * Now we do "conventional" argument parsing for a while. Note that no
     * subcommand takes arguments.
     */

    if (objc > 2) {
	Tcl_WrongNumArgs(interp, 1, objv, "subcommand");
	return TCL_ERROR;
    } else if (objc == 1) {
	index = SELF_OBJECT;
    } else if (Tcl_GetIndexFromObj(interp, objv[1], subcmds, "subcommand", 0,
	    &index) != TCL_OK) {
	return TCL_ERROR;
    }

    switch ((enum SelfCmds) index) {
    case SELF_OBJECT:
	Tcl_SetObjResult(interp, TclOOObjectName(interp, contextPtr->oPtr));
	return TCL_OK;
    case SELF_NS:
	Tcl_SetObjResult(interp, Tcl_NewStringObj(
		contextPtr->oPtr->namespacePtr->fullName,-1));
	return TCL_OK;
    case SELF_CLASS: {
	Method *mPtr = CurrentlyInvoked(contextPtr).mPtr;
	Object *declarerPtr;

	if (mPtr->declaringClassPtr != NULL) {
	    declarerPtr = mPtr->declaringClassPtr->thisPtr;
	} else if (mPtr->declaringObjectPtr != NULL) {
	    declarerPtr = mPtr->declaringObjectPtr;
	} else {
	    /*
	     * This should be unreachable code.
	     */

	    Tcl_AppendResult(interp, "method without declarer!", NULL);
	    return TCL_ERROR;
	}

	Tcl_SetObjResult(interp, TclOOObjectName(interp, declarerPtr));
	return TCL_OK;
    }
    case SELF_METHOD:
	if (contextPtr->callPtr->flags & CONSTRUCTOR) {
	    Tcl_AppendResult(interp, "<constructor>", NULL);
	} else if (contextPtr->callPtr->flags & DESTRUCTOR) {
	    Tcl_AppendResult(interp, "<destructor>", NULL);
	} else {
	    Tcl_SetObjResult(interp,
		    CurrentlyInvoked(contextPtr).mPtr->namePtr);
	}
	return TCL_OK;
    case SELF_FILTER:
	if (!CurrentlyInvoked(contextPtr).isFilter) {
	    Tcl_AppendResult(interp, "not inside a filtering context", NULL);
	    return TCL_ERROR;
	} else {
	    register struct MInvoke *miPtr = &CurrentlyInvoked(contextPtr);
	    Tcl_Obj *result[3];
	    Object *oPtr;
	    const char *type;

	    if (miPtr->filterDeclarer != NULL) {
		oPtr = miPtr->filterDeclarer->thisPtr;
		type = "class";
	    } else {
		oPtr = contextPtr->oPtr;
		type = "object";
	    }

	    result[0] = TclOOObjectName(interp, oPtr);
	    result[1] = Tcl_NewStringObj(type, -1);
	    result[2] = miPtr->mPtr->namePtr;
	    Tcl_SetObjResult(interp, Tcl_NewListObj(3, result));
	    return TCL_OK;
	}
    case SELF_CALLER:
	if ((framePtr->callerVarPtr != NULL) &&
		(framePtr->callerVarPtr->isProcCallFrame & FRAME_IS_METHOD)) {
	    CallContext *callerPtr = framePtr->callerVarPtr->clientData;
	    Method *mPtr = callerPtr->callPtr->chain[callerPtr->index].mPtr;
	    Object *declarerPtr;

	    if (mPtr->declaringClassPtr != NULL) {
		declarerPtr = mPtr->declaringClassPtr->thisPtr;
	    } else if (mPtr->declaringObjectPtr != NULL) {
		declarerPtr = mPtr->declaringObjectPtr;
	    } else {
		/*
		 * This should be unreachable code.
		 */

		Tcl_AppendResult(interp, "method without declarer!", NULL);
		return TCL_ERROR;
	    }

	    Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
		    TclOOObjectName(interp, declarerPtr));
	    Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
		    TclOOObjectName(interp, callerPtr->oPtr));
	    if (callerPtr->callPtr->flags & CONSTRUCTOR) {
		Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
			Tcl_NewStringObj("<constructor>", -1));
	    } else if (callerPtr->callPtr->flags & DESTRUCTOR) {
		Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
			Tcl_NewStringObj("<destructor>", -1));
	    } else {
		Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
			mPtr->namePtr);
	    }
	    return TCL_OK;
	} else {
	    Tcl_AppendResult(interp, "caller is not an object", NULL);
	    return TCL_ERROR;
	}
    case SELF_NEXT:
	if (contextPtr->index < contextPtr->callPtr->numChain-1) {
	    Method *mPtr =
		    contextPtr->callPtr->chain[contextPtr->index+1].mPtr;
	    Object *declarerPtr;

	    if (mPtr->declaringClassPtr != NULL) {
		declarerPtr = mPtr->declaringClassPtr->thisPtr;
	    } else if (mPtr->declaringObjectPtr != NULL) {
		declarerPtr = mPtr->declaringObjectPtr;
	    } else {
		/*
		 * This should be unreachable code.
		 */

		Tcl_AppendResult(interp, "method without declarer!", NULL);
		return TCL_ERROR;
	    }

	    Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
		    TclOOObjectName(interp, declarerPtr));
	    if (contextPtr->callPtr->flags & CONSTRUCTOR) {
		Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
			Tcl_NewStringObj("<constructor>", -1));
	    } else if (contextPtr->callPtr->flags & DESTRUCTOR) {
		Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
			Tcl_NewStringObj("<destructor>", -1));
	    } else {
		Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
			mPtr->namePtr);
	    }
	}
	return TCL_OK;
    case SELF_TARGET:
	if (!CurrentlyInvoked(contextPtr).isFilter) {
	    Tcl_AppendResult(interp, "not inside a filtering context", NULL);
	    return TCL_ERROR;
	} else {
	    Method *mPtr;
	    Object *declarerPtr;
	    int i;

	    for (i=contextPtr->index ; i<contextPtr->callPtr->numChain ; i++){
		if (!contextPtr->callPtr->chain[i].isFilter) {
		    break;
		}
	    }
	    if (i == contextPtr->callPtr->numChain) {
		Tcl_Panic("filtering call chain without terminal non-filter");
	    }
	    mPtr = contextPtr->callPtr->chain[i].mPtr;
	    if (mPtr->declaringClassPtr != NULL) {
		declarerPtr = mPtr->declaringClassPtr->thisPtr;
	    } else if (mPtr->declaringObjectPtr != NULL) {
		declarerPtr = mPtr->declaringObjectPtr;
	    } else {
		/*
		 * This should be unreachable code.
		 */

		Tcl_AppendResult(interp, "method without declarer!", NULL);
		return TCL_ERROR;
	    }
	    Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
		    TclOOObjectName(interp, declarerPtr));
	    Tcl_ListObjAppendElement(NULL, Tcl_GetObjResult(interp),
		    mPtr->namePtr);
	    return TCL_OK;
	}
    }
    return TCL_ERROR;
}

/*
 * ----------------------------------------------------------------------
 *
 * CopyObjectCmd --
 *
 *	Implementation of the [oo::copy] command, which clones an object (but
 *	not its namespace). Note that no constructors are called during this
 *	process.
 *
 * ----------------------------------------------------------------------
 */

int
TclOOCopyObjectCmd(
    ClientData clientData,
    Tcl_Interp *interp,
    int objc,
    Tcl_Obj *const *objv)
{
    Tcl_Object oPtr, o2Ptr;

    if (objc < 2 || objc > 3) {
	Tcl_WrongNumArgs(interp, 1, objv, "sourceName ?targetName?");
	return TCL_ERROR;
    }

    oPtr = Tcl_GetObjectFromObj(interp, objv[1]);
    if (oPtr == NULL) {
	return TCL_ERROR;
    }

    /*
     * Create a cloned object of the correct class. Note that constructors are
     * not called. Also note that we must resolve the object name ourselves
     * because we do not want to create the object in the current namespace,
     * but rather in the context of the namespace of the caller of the overall
     * [oo::define] command.
     */

    if (objc == 2) {
	o2Ptr = Tcl_CopyObjectInstance(interp, oPtr, NULL, NULL);
    } else {
	char *name;
	Tcl_DString buffer;

	name = TclGetString(objv[2]);
	Tcl_DStringInit(&buffer);
	if (name[0]!=':' || name[1]!=':') {
	    Interp *iPtr = (Interp *) interp;

	    if (iPtr->varFramePtr != NULL) {
		Tcl_DStringAppend(&buffer,
			iPtr->varFramePtr->nsPtr->fullName, -1);
	    }
	    Tcl_DStringAppend(&buffer, "::", 2);
	    Tcl_DStringAppend(&buffer, name, -1);
	    name = Tcl_DStringValue(&buffer);
	}
	o2Ptr = Tcl_CopyObjectInstance(interp, oPtr, name, NULL);
	Tcl_DStringFree(&buffer);
    }

    if (o2Ptr == NULL) {
	return TCL_ERROR;
    }

    /*
     * Return the name of the cloned object.
     */

    Tcl_SetObjResult(interp, TclOOObjectName(interp, (Object *) o2Ptr));
    return TCL_OK;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */