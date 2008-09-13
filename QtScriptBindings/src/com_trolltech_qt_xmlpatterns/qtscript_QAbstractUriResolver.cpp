#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValue>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <qmetaobject.h>

#include <qabstracturiresolver.h>
#include <QVariant>
#include <qabstracturiresolver.h>
#include <qbytearray.h>
#include <qcoreevent.h>
#include <qlist.h>
#include <qobject.h>
#include <qurl.h>

#include "qtscriptshell_QAbstractUriResolver.h"

static const char * const qtscript_QAbstractUriResolver_function_names[] = {
    "QAbstractUriResolver"
    // static
    // prototype
    , "resolve"
    , "toString"
};

static const char * const qtscript_QAbstractUriResolver_function_signatures[] = {
    "QObject parent"
    // static
    // prototype
    , "QUrl relative, QUrl baseURI"
""
};

static QScriptValue qtscript_QAbstractUriResolver_throw_ambiguity_error_helper(
    QScriptContext *context, const char *functionName, const char *signatures)
{
    QStringList lines = QString::fromLatin1(signatures).split(QLatin1Char('\n'));
    QStringList fullSignatures;
    for (int i = 0; i < lines.size(); ++i)
        fullSignatures.append(QString::fromLatin1("%0(%1)").arg(functionName).arg(lines.at(i)));
    return context->throwError(QString::fromLatin1("QFile::%0(): could not find a function match; candidates are:\n%1")
        .arg(functionName).arg(fullSignatures.join(QLatin1String("\n"))));
}

Q_DECLARE_METATYPE(QAbstractUriResolver*)
Q_DECLARE_METATYPE(QtScriptShell_QAbstractUriResolver*)

//
// QAbstractUriResolver
//

static QScriptValue qtscript_QAbstractUriResolver_prototype_call(QScriptContext *context, QScriptEngine *)
{
#if QT_VERSION > 0x040400
    Q_ASSERT(context->callee().isFunction());
    uint _id = context->callee().data().toUInt32();
#else
    uint _id;
    if (context->callee().isFunction())
        _id = context->callee().data().toUInt32();
    else
        _id = 0xBABE0000 + 1;
#endif
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    QAbstractUriResolver* _q_self = qscriptvalue_cast<QAbstractUriResolver*>(context->thisObject());
    if (!_q_self) {
        return context->throwError(QScriptContext::TypeError,
            QString::fromLatin1("QAbstractUriResolver.%0(): this object is not a QAbstractUriResolver")
            .arg(qtscript_QAbstractUriResolver_function_names[_id+1]));
    }

    switch (_id) {
    case 0:
    if (context->argumentCount() == 2) {
        QUrl _q_arg0 = qscriptvalue_cast<QUrl>(context->argument(0));
        QUrl _q_arg1 = qscriptvalue_cast<QUrl>(context->argument(1));
        QUrl _q_result = _q_self->resolve(_q_arg0, _q_arg1);
        return qScriptValueFromValue(context->engine(), _q_result);
    }
    break;

    case 1: {
    QString result = QString::fromLatin1("QAbstractUriResolver");
    return QScriptValue(context->engine(), result);
    }

    default:
    Q_ASSERT(false);
    }
    return qtscript_QAbstractUriResolver_throw_ambiguity_error_helper(context,
        qtscript_QAbstractUriResolver_function_names[_id+1],
        qtscript_QAbstractUriResolver_function_signatures[_id+1]);
}

static QScriptValue qtscript_QAbstractUriResolver_static_call(QScriptContext *context, QScriptEngine *)
{
    uint _id = context->callee().data().toUInt32();
    Q_ASSERT((_id & 0xFFFF0000) == 0xBABE0000);
    _id &= 0x0000FFFF;
    switch (_id) {
    case 0:
    if (context->thisObject().strictlyEquals(context->engine()->globalObject())) {
        return context->throwError(QString::fromLatin1("QAbstractUriResolver(): Did you forget to construct with 'new'?"));
    }
    if (context->argumentCount() == 0) {
        QtScriptShell_QAbstractUriResolver* _q_cpp_result = new QtScriptShell_QAbstractUriResolver();
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QAbstractUriResolver*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    } else if (context->argumentCount() == 1) {
        QObject* _q_arg0 = context->argument(0).toQObject();
        QtScriptShell_QAbstractUriResolver* _q_cpp_result = new QtScriptShell_QAbstractUriResolver(_q_arg0);
        QScriptValue _q_result = context->engine()->newQObject(context->thisObject(), (QAbstractUriResolver*)_q_cpp_result, QScriptEngine::AutoOwnership);
        _q_cpp_result->__qtscript_self = _q_result;
        return _q_result;
    }
    break;

    default:
    Q_ASSERT(false);
    }
    return qtscript_QAbstractUriResolver_throw_ambiguity_error_helper(context,
        qtscript_QAbstractUriResolver_function_names[_id],
        qtscript_QAbstractUriResolver_function_signatures[_id]);
}

static QScriptValue qtscript_QAbstractUriResolver_toScriptValue(QScriptEngine *engine, QAbstractUriResolver* const &in)
{
    return engine->newQObject(in, QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);
}

static void qtscript_QAbstractUriResolver_fromScriptValue(const QScriptValue &value, QAbstractUriResolver* &out)
{
    out = qobject_cast<QAbstractUriResolver*>(value.toQObject());
}

QScriptValue qtscript_create_QAbstractUriResolver_class(QScriptEngine *engine)
{
    static const int function_lengths[] = {
        1
        // static
        // prototype
        , 2
        , 0
    };
    engine->setDefaultPrototype(qMetaTypeId<QAbstractUriResolver*>(), QScriptValue());
    QScriptValue proto = engine->newVariant(qVariantFromValue((QAbstractUriResolver*)0));
    proto.setPrototype(engine->defaultPrototype(qMetaTypeId<QObject*>()));
    for (int i = 0; i < 2; ++i) {
        QScriptValue fun = engine->newFunction(qtscript_QAbstractUriResolver_prototype_call, function_lengths[i+1]);
        fun.setData(QScriptValue(engine, uint(0xBABE0000 + i)));
        proto.setProperty(QString::fromLatin1(qtscript_QAbstractUriResolver_function_names[i+1]),
            fun, QScriptValue::SkipInEnumeration);
    }

    qScriptRegisterMetaType<QAbstractUriResolver*>(engine, qtscript_QAbstractUriResolver_toScriptValue, 
        qtscript_QAbstractUriResolver_fromScriptValue, proto);

    QScriptValue ctor = engine->newFunction(qtscript_QAbstractUriResolver_static_call, proto, function_lengths[0]);
    ctor.setData(QScriptValue(engine, uint(0xBABE0000 + 0)));

    return ctor;
}