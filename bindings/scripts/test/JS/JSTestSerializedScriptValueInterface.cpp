/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(Condition1) || ENABLE(Condition2)

#include "JSTestSerializedScriptValueInterface.h"

#include "ExceptionCode.h"
#include "JSArray.h"
#include "JSDOMBinding.h"
#include "JSMessagePortArray.h"
#include "MessagePortArray.h"
#include "SerializedScriptValue.h"
#include "TestSerializedScriptValueInterface.h"
#include <runtime/Error.h>
#include <wtf/GetPtr.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSTestSerializedScriptValueInterface);
/* Hash table */

static const HashTableValue JSTestSerializedScriptValueInterfaceTableValues[] =
{
    { "value", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializedScriptValueInterfaceValue), (intptr_t)setJSTestSerializedScriptValueInterfaceValue, NoIntrinsic },
    { "readonlyValue", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializedScriptValueInterfaceReadonlyValue), (intptr_t)0, NoIntrinsic },
    { "cachedValue", DontDelete, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializedScriptValueInterfaceCachedValue), (intptr_t)setJSTestSerializedScriptValueInterfaceCachedValue, NoIntrinsic },
    { "ports", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializedScriptValueInterfacePorts), (intptr_t)0, NoIntrinsic },
    { "cachedReadonlyValue", DontDelete | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializedScriptValueInterfaceCachedReadonlyValue), (intptr_t)0, NoIntrinsic },
    { "constructor", DontEnum | ReadOnly, (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestSerializedScriptValueInterfaceConstructor), (intptr_t)0, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSTestSerializedScriptValueInterfaceTable = { 17, 15, JSTestSerializedScriptValueInterfaceTableValues, 0 };
/* Hash table for constructor */

static const HashTableValue JSTestSerializedScriptValueInterfaceConstructorTableValues[] =
{
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSTestSerializedScriptValueInterfaceConstructorTable = { 1, 0, JSTestSerializedScriptValueInterfaceConstructorTableValues, 0 };
const ClassInfo JSTestSerializedScriptValueInterfaceConstructor::s_info = { "TestSerializedScriptValueInterfaceConstructor", &Base::s_info, &JSTestSerializedScriptValueInterfaceConstructorTable, 0, CREATE_METHOD_TABLE(JSTestSerializedScriptValueInterfaceConstructor) };

JSTestSerializedScriptValueInterfaceConstructor::JSTestSerializedScriptValueInterfaceConstructor(Structure* structure, JSDOMGlobalObject* globalObject)
    : DOMConstructorObject(structure, globalObject)
{
}

void JSTestSerializedScriptValueInterfaceConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSTestSerializedScriptValueInterfacePrototype::self(exec, globalObject), DontDelete | ReadOnly);
    putDirect(exec->globalData(), exec->propertyNames().length, jsNumber(3), ReadOnly | DontDelete | DontEnum);
}

bool JSTestSerializedScriptValueInterfaceConstructor::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSTestSerializedScriptValueInterfaceConstructor, JSDOMWrapper>(exec, &JSTestSerializedScriptValueInterfaceConstructorTable, jsCast<JSTestSerializedScriptValueInterfaceConstructor*>(cell), propertyName, slot);
}

bool JSTestSerializedScriptValueInterfaceConstructor::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticValueDescriptor<JSTestSerializedScriptValueInterfaceConstructor, JSDOMWrapper>(exec, &JSTestSerializedScriptValueInterfaceConstructorTable, jsCast<JSTestSerializedScriptValueInterfaceConstructor*>(object), propertyName, descriptor);
}

EncodedJSValue JSC_HOST_CALL JSTestSerializedScriptValueInterfaceConstructor::constructJSTestSerializedScriptValueInterface(ExecState* exec)
{
    JSTestSerializedScriptValueInterfaceConstructor* castedThis = jsCast<JSTestSerializedScriptValueInterfaceConstructor*>(exec->callee());
    if (exec->argumentCount() < 2)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    const String& hello(ustringToString(MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).isEmpty() ? UString() : MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined).toString(exec)->value(exec)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    RefPtr<SerializedScriptValue> data(SerializedScriptValue::create(exec, MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    Array* transferList(toArray(MAYBE_MISSING_PARAMETER(exec, 2, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    RefPtr<TestSerializedScriptValueInterface> object = TestSerializedScriptValueInterface::create(hello, data, transferList);
    return JSValue::encode(asObject(toJS(exec, castedThis->globalObject(), object.get())));
}

ConstructType JSTestSerializedScriptValueInterfaceConstructor::getConstructData(JSCell*, ConstructData& constructData)
{
    constructData.native.function = constructJSTestSerializedScriptValueInterface;
    return ConstructTypeHost;
}

/* Hash table for prototype */

static const HashTableValue JSTestSerializedScriptValueInterfacePrototypeTableValues[] =
{
    { "acceptTransferList", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsTestSerializedScriptValueInterfacePrototypeFunctionAcceptTransferList), (intptr_t)2, NoIntrinsic },
    { "multiTransferList", DontDelete | JSC::Function, (intptr_t)static_cast<NativeFunction>(jsTestSerializedScriptValueInterfacePrototypeFunctionMultiTransferList), (intptr_t)4, NoIntrinsic },
    { 0, 0, 0, 0, NoIntrinsic }
};

static const HashTable JSTestSerializedScriptValueInterfacePrototypeTable = { 5, 3, JSTestSerializedScriptValueInterfacePrototypeTableValues, 0 };
const ClassInfo JSTestSerializedScriptValueInterfacePrototype::s_info = { "TestSerializedScriptValueInterfacePrototype", &Base::s_info, &JSTestSerializedScriptValueInterfacePrototypeTable, 0, CREATE_METHOD_TABLE(JSTestSerializedScriptValueInterfacePrototype) };

JSObject* JSTestSerializedScriptValueInterfacePrototype::self(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMPrototype<JSTestSerializedScriptValueInterface>(exec, globalObject);
}

bool JSTestSerializedScriptValueInterfacePrototype::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestSerializedScriptValueInterfacePrototype* thisObject = jsCast<JSTestSerializedScriptValueInterfacePrototype*>(cell);
    return getStaticFunctionSlot<JSObject>(exec, &JSTestSerializedScriptValueInterfacePrototypeTable, thisObject, propertyName, slot);
}

bool JSTestSerializedScriptValueInterfacePrototype::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestSerializedScriptValueInterfacePrototype* thisObject = jsCast<JSTestSerializedScriptValueInterfacePrototype*>(object);
    return getStaticFunctionDescriptor<JSObject>(exec, &JSTestSerializedScriptValueInterfacePrototypeTable, thisObject, propertyName, descriptor);
}

const ClassInfo JSTestSerializedScriptValueInterface::s_info = { "TestSerializedScriptValueInterface", &Base::s_info, &JSTestSerializedScriptValueInterfaceTable, 0 , CREATE_METHOD_TABLE(JSTestSerializedScriptValueInterface) };

JSTestSerializedScriptValueInterface::JSTestSerializedScriptValueInterface(Structure* structure, JSDOMGlobalObject* globalObject, PassRefPtr<TestSerializedScriptValueInterface> impl)
    : JSDOMWrapper(structure, globalObject)
    , m_impl(impl.leakRef())
{
}

void JSTestSerializedScriptValueInterface::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    ASSERT(inherits(&s_info));
}

JSObject* JSTestSerializedScriptValueInterface::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSTestSerializedScriptValueInterfacePrototype::create(exec->globalData(), globalObject, JSTestSerializedScriptValueInterfacePrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}

void JSTestSerializedScriptValueInterface::destroy(JSC::JSCell* cell)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(cell);
    thisObject->JSTestSerializedScriptValueInterface::~JSTestSerializedScriptValueInterface();
}

JSTestSerializedScriptValueInterface::~JSTestSerializedScriptValueInterface()
{
    releaseImplIfNotNull();
}

bool JSTestSerializedScriptValueInterface::getOwnPropertySlot(JSCell* cell, ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueSlot<JSTestSerializedScriptValueInterface, Base>(exec, &JSTestSerializedScriptValueInterfaceTable, thisObject, propertyName, slot);
}

bool JSTestSerializedScriptValueInterface::getOwnPropertyDescriptor(JSObject* object, ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    return getStaticValueDescriptor<JSTestSerializedScriptValueInterface, Base>(exec, &JSTestSerializedScriptValueInterfaceTable, thisObject, propertyName, descriptor);
}

JSValue jsTestSerializedScriptValueInterfaceValue(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    JSValue result = impl->value() ? impl->value()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    return result;
}


JSValue jsTestSerializedScriptValueInterfaceReadonlyValue(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    JSValue result = impl->readonlyValue() ? impl->readonlyValue()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    return result;
}


JSValue jsTestSerializedScriptValueInterfaceCachedValue(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    if (JSValue cachedValue = castedThis->m_cachedValue.get())
        return cachedValue;
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    JSValue result = impl->cachedValue() ? impl->cachedValue()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    castedThis->m_cachedValue.set(exec->globalData(), castedThis, result);
    return result;
}


JSValue jsTestSerializedScriptValueInterfacePorts(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    JSValue result = toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->ports()));
    return result;
}


JSValue jsTestSerializedScriptValueInterfaceCachedReadonlyValue(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    UNUSED_PARAM(exec);
    if (JSValue cachedValue = castedThis->m_cachedReadonlyValue.get())
        return cachedValue;
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    JSValue result = impl->cachedReadonlyValue() ? impl->cachedReadonlyValue()->deserialize(exec, castedThis->globalObject(), 0) : jsNull();
    castedThis->m_cachedReadonlyValue.set(exec->globalData(), castedThis, result);
    return result;
}


JSValue jsTestSerializedScriptValueInterfaceConstructor(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestSerializedScriptValueInterface* domObject = jsCast<JSTestSerializedScriptValueInterface*>(asObject(slotBase));
    return JSTestSerializedScriptValueInterface::getConstructor(exec, domObject->globalObject());
}

void JSTestSerializedScriptValueInterface::put(JSCell* cell, ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSTestSerializedScriptValueInterface, Base>(exec, propertyName, value, &JSTestSerializedScriptValueInterfaceTable, thisObject, slot);
}

void setJSTestSerializedScriptValueInterfaceValue(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(thisObject);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    impl->setValue(SerializedScriptValue::create(exec, value));
}


void setJSTestSerializedScriptValueInterfaceCachedValue(ExecState* exec, JSObject* thisObject, JSValue value)
{
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(thisObject);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    impl->setCachedValue(SerializedScriptValue::create(exec, value));
}


JSValue JSTestSerializedScriptValueInterface::getConstructor(ExecState* exec, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestSerializedScriptValueInterfaceConstructor>(exec, jsCast<JSDOMGlobalObject*>(globalObject));
}

EncodedJSValue JSC_HOST_CALL jsTestSerializedScriptValueInterfacePrototypeFunctionAcceptTransferList(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestSerializedScriptValueInterface::s_info))
        return throwVMTypeError(exec);
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestSerializedScriptValueInterface::s_info);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());
    if (exec->argumentCount() < 1)
        return throwVMError(exec, createNotEnoughArgumentsError(exec));
    RefPtr<SerializedScriptValue> data(SerializedScriptValue::create(exec, MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());

    size_t argsCount = exec->argumentCount();
    if (argsCount <= 1) {
        impl->acceptTransferList(data);
        return JSValue::encode(jsUndefined());
    }

    Array* transferList(toArray(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->acceptTransferList(data, transferList);
    return JSValue::encode(jsUndefined());
}

EncodedJSValue JSC_HOST_CALL jsTestSerializedScriptValueInterfacePrototypeFunctionMultiTransferList(ExecState* exec)
{
    JSValue thisValue = exec->hostThisValue();
    if (!thisValue.inherits(&JSTestSerializedScriptValueInterface::s_info))
        return throwVMTypeError(exec);
    JSTestSerializedScriptValueInterface* castedThis = jsCast<JSTestSerializedScriptValueInterface*>(asObject(thisValue));
    ASSERT_GC_OBJECT_INHERITS(castedThis, &JSTestSerializedScriptValueInterface::s_info);
    TestSerializedScriptValueInterface* impl = static_cast<TestSerializedScriptValueInterface*>(castedThis->impl());

    size_t argsCount = exec->argumentCount();
    if (argsCount <= 0) {
        impl->multiTransferList();
        return JSValue::encode(jsUndefined());
    }

    RefPtr<SerializedScriptValue> first(SerializedScriptValue::create(exec, MAYBE_MISSING_PARAMETER(exec, 0, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    if (argsCount <= 1) {
        impl->multiTransferList(first);
        return JSValue::encode(jsUndefined());
    }

    Array* tx(toArray(MAYBE_MISSING_PARAMETER(exec, 1, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    if (argsCount <= 2) {
        impl->multiTransferList(first, tx);
        return JSValue::encode(jsUndefined());
    }

    RefPtr<SerializedScriptValue> second(SerializedScriptValue::create(exec, MAYBE_MISSING_PARAMETER(exec, 2, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    if (argsCount <= 3) {
        impl->multiTransferList(first, tx, second);
        return JSValue::encode(jsUndefined());
    }

    Array* txx(toArray(MAYBE_MISSING_PARAMETER(exec, 3, DefaultIsUndefined)));
    if (exec->hadException())
        return JSValue::encode(jsUndefined());
    impl->multiTransferList(first, tx, second, txx);
    return JSValue::encode(jsUndefined());
}

void JSTestSerializedScriptValueInterface::visitChildren(JSCell* cell, SlotVisitor& visitor)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    COMPILE_ASSERT(StructureFlags & OverridesVisitChildren, OverridesVisitChildrenWithoutSettingFlag);
    ASSERT(thisObject->structure()->typeInfo().overridesVisitChildren());
    Base::visitChildren(thisObject, visitor);
    if (thisObject->m_cachedValue)
        visitor.append(&thisObject->m_cachedValue);
    if (thisObject->m_cachedReadonlyValue)
        visitor.append(&thisObject->m_cachedReadonlyValue);
}

static inline bool isObservable(JSTestSerializedScriptValueInterface* jsTestSerializedScriptValueInterface)
{
    if (jsTestSerializedScriptValueInterface->hasCustomProperties())
        return true;
    return false;
}

bool JSTestSerializedScriptValueInterfaceOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSTestSerializedScriptValueInterface* jsTestSerializedScriptValueInterface = jsCast<JSTestSerializedScriptValueInterface*>(handle.get().asCell());
    if (!isObservable(jsTestSerializedScriptValueInterface))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}

void JSTestSerializedScriptValueInterfaceOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestSerializedScriptValueInterface* jsTestSerializedScriptValueInterface = jsCast<JSTestSerializedScriptValueInterface*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsTestSerializedScriptValueInterface->impl(), jsTestSerializedScriptValueInterface);
    jsTestSerializedScriptValueInterface->releaseImpl();
}

JSC::JSValue toJS(JSC::ExecState* exec, JSDOMGlobalObject* globalObject, TestSerializedScriptValueInterface* impl)
{
    return wrap<JSTestSerializedScriptValueInterface>(exec, globalObject, impl);
}

TestSerializedScriptValueInterface* toTestSerializedScriptValueInterface(JSC::JSValue value)
{
    return value.inherits(&JSTestSerializedScriptValueInterface::s_info) ? jsCast<JSTestSerializedScriptValueInterface*>(asObject(value))->impl() : 0;
}

}

#endif // ENABLE(Condition1) || ENABLE(Condition2)
