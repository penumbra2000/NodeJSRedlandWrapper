#include <libpq-fe.h>
#include "type-oids.h"
#include <node.h>
#include <node_events.h>
#include <assert.h>
#include <stdlib.h>

using namespace v8;
using namespace node;

class World : public EventEmitter {
  public:
    static Persistent<String> RasqalSym, LoggerSym, DigestSym, FeaturesSym;

    static void Initialize (v8::Handle<v8::Object> target) {
      HandleScope scope; // TODO: Does this do anything?

      Local<FunctionTemplate> t = FunctionTemplate::New(New);
      t->Inherit(EventEmitter::constructor_template);
      t->InstanceTemplate()->SetInternalFieldCount(1);

      // Just symbols?
      RasqalSym = NODE_PSYMBOL("rasqal");
      LoggerSym = NODE_PSYMBOL("logger");
      DigestSym = NODE_PSYMBOL("digest");
      FeaturesSym = NODE_PSYMBOL("features");

      // Methods to expose for JS -- none for this module, all properties
      // NODE_SET_PROTOTYPE_METHOD(t, "setDigest", SetDigest);

      // Set up accessors/mutators for properties
      t->PrototypeTemplate()->SetAccessor(RasqalSym, GetRasqal, SetRasqal);
      t->PrototypeTemplate()->SetAccessor(LoggerSym, SetLogger);
      t->PrototypeTemplate()->SetAccessor(DigestSym, SetDigest);
      t->PrototypeTemplate()->SetAccessor(FeaturesSym, GetFeatures, SetFeatures);

      // TODO: Implement stub classes for logger, features functions

      // Pass the wrapper prototype back to the caller
      target->Set(String::NewSymbol("World"), t->GetFunction());
    }

  protected:
    librdf_world *world;

    // Constructor implementation
    static Handle<Value> New(const Arguments &args) { 
      HandleScope scope; // TODO: Does this do anything?

      World *world = new World();
      world->Wrap(args.This());

      // TODO: Should we populate our underlying world ob here?
      // TODO: Redland 1.0.6 and earlier require librdf_world_open() call
      // Should we call this here?

      return args.This();
    }

    static Handle<Value> GetRasqal(Local<String> property, const AccessorInfo &info) { 
      World *world = ObjectWrap::Unwrap<World>(args.This());

      assert(world);
      assert(property==RasqalSym);

      HandleScope scope;
      
      Handle<Value> result = Rasqal::New(rasqal);
      return scope.Close(result);
    }

    static Handle<Value> SetRasqal(const Arguments &args) { }

    static Handle<Value> InitMutex(const Arguments &args) { }
    static Handle<Value> SetError(const Arguments &args) { }
    static Handle<Value> SetWarning(const Arguments &args) { }
    static Handle<Value> SetLogger(const Arguments &args) { }
    static Handle<Value> SetDigest(const Arguments &args) { }
    static Handle<Value> GetFeature(const Arguments &args) { }
    static Handle<Value> SetFeature(const Arguments &args) { }
}
