// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: person.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace People {

  /// <summary>Holder for reflection information generated from person.proto</summary>
  public static partial class PersonReflection {

    #region Descriptor
    /// <summary>File descriptor for person.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static PersonReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "CgxwZXJzb24ucHJvdG8SBlBlb3BsZRoMYW5pbWFsLnByb3RvGgtob3VzZS5w",
            "cm90byKyAQoGUGVyc29uEgoKAmlkGAEgASgFEgwKBG5hbWUYAiABKAkSIwoF",
            "c3R5cGUYAyABKA4yFC5QZW9wbGUuUGVyc29uLlNUeXBlEg0KBWVtYWlsGAQg",
            "ASgJEhgKA2RvZxgFIAEoCzILLkFuaW1hbC5Eb2cSIQoFaG91c2UYBiABKAsy",
            "Ei5FbnZpcm9ubWVudC5Ib3VzZSIdCgVTVHlwZRIICgRNQUxFEAASCgoGRkVN",
            "QUxFEAFiBnByb3RvMw=="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { global::Animal.AnimalReflection.Descriptor, global::Environment.HouseReflection.Descriptor, },
          new pbr::GeneratedClrTypeInfo(null, null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::People.Person), global::People.Person.Parser, new[]{ "Id", "Name", "Stype", "Email", "Dog", "House" }, null, new[]{ typeof(global::People.Person.Types.SType) }, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  public sealed partial class Person : pb::IMessage<Person> {
    private static readonly pb::MessageParser<Person> _parser = new pb::MessageParser<Person>(() => new Person());
    private pb::UnknownFieldSet _unknownFields;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<Person> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::People.PersonReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Person() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Person(Person other) : this() {
      id_ = other.id_;
      name_ = other.name_;
      stype_ = other.stype_;
      email_ = other.email_;
      dog_ = other.dog_ != null ? other.dog_.Clone() : null;
      house_ = other.house_ != null ? other.house_.Clone() : null;
      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public Person Clone() {
      return new Person(this);
    }

    /// <summary>Field number for the "id" field.</summary>
    public const int IdFieldNumber = 1;
    private int id_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int Id {
      get { return id_; }
      set {
        id_ = value;
      }
    }

    /// <summary>Field number for the "name" field.</summary>
    public const int NameFieldNumber = 2;
    private string name_ = "";
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string Name {
      get { return name_; }
      set {
        name_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    /// <summary>Field number for the "stype" field.</summary>
    public const int StypeFieldNumber = 3;
    private global::People.Person.Types.SType stype_ = global::People.Person.Types.SType.Male;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::People.Person.Types.SType Stype {
      get { return stype_; }
      set {
        stype_ = value;
      }
    }

    /// <summary>Field number for the "email" field.</summary>
    public const int EmailFieldNumber = 4;
    private string email_ = "";
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string Email {
      get { return email_; }
      set {
        email_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    /// <summary>Field number for the "dog" field.</summary>
    public const int DogFieldNumber = 5;
    private global::Animal.Dog dog_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Animal.Dog Dog {
      get { return dog_; }
      set {
        dog_ = value;
      }
    }

    /// <summary>Field number for the "house" field.</summary>
    public const int HouseFieldNumber = 6;
    private global::Environment.House house_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Environment.House House {
      get { return house_; }
      set {
        house_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as Person);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(Person other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (Id != other.Id) return false;
      if (Name != other.Name) return false;
      if (Stype != other.Stype) return false;
      if (Email != other.Email) return false;
      if (!object.Equals(Dog, other.Dog)) return false;
      if (!object.Equals(House, other.House)) return false;
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (Id != 0) hash ^= Id.GetHashCode();
      if (Name.Length != 0) hash ^= Name.GetHashCode();
      if (Stype != global::People.Person.Types.SType.Male) hash ^= Stype.GetHashCode();
      if (Email.Length != 0) hash ^= Email.GetHashCode();
      if (dog_ != null) hash ^= Dog.GetHashCode();
      if (house_ != null) hash ^= House.GetHashCode();
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (Id != 0) {
        output.WriteRawTag(8);
        output.WriteInt32(Id);
      }
      if (Name.Length != 0) {
        output.WriteRawTag(18);
        output.WriteString(Name);
      }
      if (Stype != global::People.Person.Types.SType.Male) {
        output.WriteRawTag(24);
        output.WriteEnum((int) Stype);
      }
      if (Email.Length != 0) {
        output.WriteRawTag(34);
        output.WriteString(Email);
      }
      if (dog_ != null) {
        output.WriteRawTag(42);
        output.WriteMessage(Dog);
      }
      if (house_ != null) {
        output.WriteRawTag(50);
        output.WriteMessage(House);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (Id != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Id);
      }
      if (Name.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(Name);
      }
      if (Stype != global::People.Person.Types.SType.Male) {
        size += 1 + pb::CodedOutputStream.ComputeEnumSize((int) Stype);
      }
      if (Email.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(Email);
      }
      if (dog_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(Dog);
      }
      if (house_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(House);
      }
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(Person other) {
      if (other == null) {
        return;
      }
      if (other.Id != 0) {
        Id = other.Id;
      }
      if (other.Name.Length != 0) {
        Name = other.Name;
      }
      if (other.Stype != global::People.Person.Types.SType.Male) {
        Stype = other.Stype;
      }
      if (other.Email.Length != 0) {
        Email = other.Email;
      }
      if (other.dog_ != null) {
        if (dog_ == null) {
          Dog = new global::Animal.Dog();
        }
        Dog.MergeFrom(other.Dog);
      }
      if (other.house_ != null) {
        if (house_ == null) {
          House = new global::Environment.House();
        }
        House.MergeFrom(other.House);
      }
      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
            break;
          case 8: {
            Id = input.ReadInt32();
            break;
          }
          case 18: {
            Name = input.ReadString();
            break;
          }
          case 24: {
            Stype = (global::People.Person.Types.SType) input.ReadEnum();
            break;
          }
          case 34: {
            Email = input.ReadString();
            break;
          }
          case 42: {
            if (dog_ == null) {
              Dog = new global::Animal.Dog();
            }
            input.ReadMessage(Dog);
            break;
          }
          case 50: {
            if (house_ == null) {
              House = new global::Environment.House();
            }
            input.ReadMessage(House);
            break;
          }
        }
      }
    }

    #region Nested types
    /// <summary>Container for nested types declared in the Person message type.</summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static partial class Types {
      public enum SType {
        [pbr::OriginalName("MALE")] Male = 0,
        [pbr::OriginalName("FEMALE")] Female = 1,
      }

    }
    #endregion

  }

  #endregion

}

#endregion Designer generated code