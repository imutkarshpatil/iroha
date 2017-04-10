// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ASSET_IROHA_H_
#define FLATBUFFERS_GENERATED_ASSET_IROHA_H_

#include "flatbuffers/flatbuffers.h"

#include "primitives_generated.h"

namespace iroha {

struct ComplexAsset;

struct ComplexAssetLogic;

struct Currency;

struct Asset;

enum AnyAsset {
  AnyAsset_NONE = 0,
  AnyAsset_ComplexAsset = 1,
  AnyAsset_Currency = 2,
  AnyAsset_MIN = AnyAsset_NONE,
  AnyAsset_MAX = AnyAsset_Currency
};

inline const char **EnumNamesAnyAsset() {
  static const char *names[] = {
    "NONE",
    "ComplexAsset",
    "Currency",
    nullptr
  };
  return names;
}

inline const char *EnumNameAnyAsset(AnyAsset e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesAnyAsset()[index];
}

template<typename T> struct AnyAssetTraits {
  static const AnyAsset enum_value = AnyAsset_NONE;
};

template<> struct AnyAssetTraits<ComplexAsset> {
  static const AnyAsset enum_value = AnyAsset_ComplexAsset;
};

template<> struct AnyAssetTraits<Currency> {
  static const AnyAsset enum_value = AnyAsset_Currency;
};

bool VerifyAnyAsset(flatbuffers::Verifier &verifier, const void *obj, AnyAsset type);
bool VerifyAnyAssetVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

struct ComplexAsset FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ASSET_NAME = 4,
    VT_DOMAIN_NAME = 6,
    VT_LEDGER_NAME = 8,
    VT_DESCRIPTION = 10,
    VT_PROP = 12,
    VT_LOGIC = 14
  };
  const flatbuffers::String *asset_name() const {
    return GetPointer<const flatbuffers::String *>(VT_ASSET_NAME);
  }
  bool KeyCompareLessThan(const ComplexAsset *o) const {
    return *asset_name() < *o->asset_name();
  }
  int KeyCompareWithValue(const char *val) const {
    return strcmp(asset_name()->c_str(), val);
  }
  const flatbuffers::String *domain_name() const {
    return GetPointer<const flatbuffers::String *>(VT_DOMAIN_NAME);
  }
  const flatbuffers::String *ledger_name() const {
    return GetPointer<const flatbuffers::String *>(VT_LEDGER_NAME);
  }
  const flatbuffers::String *description() const {
    return GetPointer<const flatbuffers::String *>(VT_DESCRIPTION);
  }
  const flatbuffers::Vector<flatbuffers::Offset<iroha::KeyValueObject>> *prop() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<iroha::KeyValueObject>> *>(VT_PROP);
  }
  const ComplexAssetLogic *logic() const {
    return GetPointer<const ComplexAssetLogic *>(VT_LOGIC);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, VT_ASSET_NAME) &&
           verifier.Verify(asset_name()) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, VT_DOMAIN_NAME) &&
           verifier.Verify(domain_name()) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, VT_LEDGER_NAME) &&
           verifier.Verify(ledger_name()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_DESCRIPTION) &&
           verifier.Verify(description()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_PROP) &&
           verifier.Verify(prop()) &&
           verifier.VerifyVectorOfTables(prop()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_LOGIC) &&
           verifier.VerifyTable(logic()) &&
           verifier.EndTable();
  }
};

struct ComplexAssetBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_asset_name(flatbuffers::Offset<flatbuffers::String> asset_name) {
    fbb_.AddOffset(ComplexAsset::VT_ASSET_NAME, asset_name);
  }
  void add_domain_name(flatbuffers::Offset<flatbuffers::String> domain_name) {
    fbb_.AddOffset(ComplexAsset::VT_DOMAIN_NAME, domain_name);
  }
  void add_ledger_name(flatbuffers::Offset<flatbuffers::String> ledger_name) {
    fbb_.AddOffset(ComplexAsset::VT_LEDGER_NAME, ledger_name);
  }
  void add_description(flatbuffers::Offset<flatbuffers::String> description) {
    fbb_.AddOffset(ComplexAsset::VT_DESCRIPTION, description);
  }
  void add_prop(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<iroha::KeyValueObject>>> prop) {
    fbb_.AddOffset(ComplexAsset::VT_PROP, prop);
  }
  void add_logic(flatbuffers::Offset<ComplexAssetLogic> logic) {
    fbb_.AddOffset(ComplexAsset::VT_LOGIC, logic);
  }
  ComplexAssetBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ComplexAssetBuilder &operator=(const ComplexAssetBuilder &);
  flatbuffers::Offset<ComplexAsset> Finish() {
    const auto end = fbb_.EndTable(start_, 6);
    auto o = flatbuffers::Offset<ComplexAsset>(end);
    fbb_.Required(o, ComplexAsset::VT_ASSET_NAME);
    fbb_.Required(o, ComplexAsset::VT_DOMAIN_NAME);
    fbb_.Required(o, ComplexAsset::VT_LEDGER_NAME);
    return o;
  }
};

inline flatbuffers::Offset<ComplexAsset> CreateComplexAsset(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> asset_name = 0,
    flatbuffers::Offset<flatbuffers::String> domain_name = 0,
    flatbuffers::Offset<flatbuffers::String> ledger_name = 0,
    flatbuffers::Offset<flatbuffers::String> description = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<iroha::KeyValueObject>>> prop = 0,
    flatbuffers::Offset<ComplexAssetLogic> logic = 0) {
  ComplexAssetBuilder builder_(_fbb);
  builder_.add_logic(logic);
  builder_.add_prop(prop);
  builder_.add_description(description);
  builder_.add_ledger_name(ledger_name);
  builder_.add_domain_name(domain_name);
  builder_.add_asset_name(asset_name);
  return builder_.Finish();
}

inline flatbuffers::Offset<ComplexAsset> CreateComplexAssetDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *asset_name = nullptr,
    const char *domain_name = nullptr,
    const char *ledger_name = nullptr,
    const char *description = nullptr,
    const std::vector<flatbuffers::Offset<iroha::KeyValueObject>> *prop = nullptr,
    flatbuffers::Offset<ComplexAssetLogic> logic = 0) {
  return iroha::CreateComplexAsset(
      _fbb,
      asset_name ? _fbb.CreateString(asset_name) : 0,
      domain_name ? _fbb.CreateString(domain_name) : 0,
      ledger_name ? _fbb.CreateString(ledger_name) : 0,
      description ? _fbb.CreateString(description) : 0,
      prop ? _fbb.CreateVector<flatbuffers::Offset<iroha::KeyValueObject>>(*prop) : 0,
      logic);
}

struct ComplexAssetLogic FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ADD = 4,
    VT_REMOVE = 6,
    VT_TRANSFER = 8
  };
  const iroha::Chaincode *add() const {
    return GetPointer<const iroha::Chaincode *>(VT_ADD);
  }
  const iroha::Chaincode *remove() const {
    return GetPointer<const iroha::Chaincode *>(VT_REMOVE);
  }
  const iroha::Chaincode *transfer() const {
    return GetPointer<const iroha::Chaincode *>(VT_TRANSFER);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_ADD) &&
           verifier.VerifyTable(add()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_REMOVE) &&
           verifier.VerifyTable(remove()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TRANSFER) &&
           verifier.VerifyTable(transfer()) &&
           verifier.EndTable();
  }
};

struct ComplexAssetLogicBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_add(flatbuffers::Offset<iroha::Chaincode> add) {
    fbb_.AddOffset(ComplexAssetLogic::VT_ADD, add);
  }
  void add_remove(flatbuffers::Offset<iroha::Chaincode> remove) {
    fbb_.AddOffset(ComplexAssetLogic::VT_REMOVE, remove);
  }
  void add_transfer(flatbuffers::Offset<iroha::Chaincode> transfer) {
    fbb_.AddOffset(ComplexAssetLogic::VT_TRANSFER, transfer);
  }
  ComplexAssetLogicBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ComplexAssetLogicBuilder &operator=(const ComplexAssetLogicBuilder &);
  flatbuffers::Offset<ComplexAssetLogic> Finish() {
    const auto end = fbb_.EndTable(start_, 3);
    auto o = flatbuffers::Offset<ComplexAssetLogic>(end);
    return o;
  }
};

inline flatbuffers::Offset<ComplexAssetLogic> CreateComplexAssetLogic(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<iroha::Chaincode> add = 0,
    flatbuffers::Offset<iroha::Chaincode> remove = 0,
    flatbuffers::Offset<iroha::Chaincode> transfer = 0) {
  ComplexAssetLogicBuilder builder_(_fbb);
  builder_.add_transfer(transfer);
  builder_.add_remove(remove);
  builder_.add_add(add);
  return builder_.Finish();
}

struct Currency FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_CURRENCY_NAME = 4,
    VT_DOMAIN_NAME = 6,
    VT_LEDGER_NAME = 8,
    VT_DESCRIPTION = 10,
    VT_AMOUNT = 12,
    VT_PRECISION = 14
  };
  const flatbuffers::String *currency_name() const {
    return GetPointer<const flatbuffers::String *>(VT_CURRENCY_NAME);
  }
  bool KeyCompareLessThan(const Currency *o) const {
    return *currency_name() < *o->currency_name();
  }
  int KeyCompareWithValue(const char *val) const {
    return strcmp(currency_name()->c_str(), val);
  }
  const flatbuffers::String *domain_name() const {
    return GetPointer<const flatbuffers::String *>(VT_DOMAIN_NAME);
  }
  const flatbuffers::String *ledger_name() const {
    return GetPointer<const flatbuffers::String *>(VT_LEDGER_NAME);
  }
  const flatbuffers::String *description() const {
    return GetPointer<const flatbuffers::String *>(VT_DESCRIPTION);
  }
  uint64_t amount() const {
    return GetField<uint64_t>(VT_AMOUNT, 0);
  }
  uint8_t precision() const {
    return GetField<uint8_t>(VT_PRECISION, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, VT_CURRENCY_NAME) &&
           verifier.Verify(currency_name()) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, VT_DOMAIN_NAME) &&
           verifier.Verify(domain_name()) &&
           VerifyFieldRequired<flatbuffers::uoffset_t>(verifier, VT_LEDGER_NAME) &&
           verifier.Verify(ledger_name()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_DESCRIPTION) &&
           verifier.Verify(description()) &&
           VerifyField<uint64_t>(verifier, VT_AMOUNT) &&
           VerifyField<uint8_t>(verifier, VT_PRECISION) &&
           verifier.EndTable();
  }
};

struct CurrencyBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_currency_name(flatbuffers::Offset<flatbuffers::String> currency_name) {
    fbb_.AddOffset(Currency::VT_CURRENCY_NAME, currency_name);
  }
  void add_domain_name(flatbuffers::Offset<flatbuffers::String> domain_name) {
    fbb_.AddOffset(Currency::VT_DOMAIN_NAME, domain_name);
  }
  void add_ledger_name(flatbuffers::Offset<flatbuffers::String> ledger_name) {
    fbb_.AddOffset(Currency::VT_LEDGER_NAME, ledger_name);
  }
  void add_description(flatbuffers::Offset<flatbuffers::String> description) {
    fbb_.AddOffset(Currency::VT_DESCRIPTION, description);
  }
  void add_amount(uint64_t amount) {
    fbb_.AddElement<uint64_t>(Currency::VT_AMOUNT, amount, 0);
  }
  void add_precision(uint8_t precision) {
    fbb_.AddElement<uint8_t>(Currency::VT_PRECISION, precision, 0);
  }
  CurrencyBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  CurrencyBuilder &operator=(const CurrencyBuilder &);
  flatbuffers::Offset<Currency> Finish() {
    const auto end = fbb_.EndTable(start_, 6);
    auto o = flatbuffers::Offset<Currency>(end);
    fbb_.Required(o, Currency::VT_CURRENCY_NAME);
    fbb_.Required(o, Currency::VT_DOMAIN_NAME);
    fbb_.Required(o, Currency::VT_LEDGER_NAME);
    return o;
  }
};

inline flatbuffers::Offset<Currency> CreateCurrency(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> currency_name = 0,
    flatbuffers::Offset<flatbuffers::String> domain_name = 0,
    flatbuffers::Offset<flatbuffers::String> ledger_name = 0,
    flatbuffers::Offset<flatbuffers::String> description = 0,
    uint64_t amount = 0,
    uint8_t precision = 0) {
  CurrencyBuilder builder_(_fbb);
  builder_.add_amount(amount);
  builder_.add_description(description);
  builder_.add_ledger_name(ledger_name);
  builder_.add_domain_name(domain_name);
  builder_.add_currency_name(currency_name);
  builder_.add_precision(precision);
  return builder_.Finish();
}

inline flatbuffers::Offset<Currency> CreateCurrencyDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *currency_name = nullptr,
    const char *domain_name = nullptr,
    const char *ledger_name = nullptr,
    const char *description = nullptr,
    uint64_t amount = 0,
    uint8_t precision = 0) {
  return iroha::CreateCurrency(
      _fbb,
      currency_name ? _fbb.CreateString(currency_name) : 0,
      domain_name ? _fbb.CreateString(domain_name) : 0,
      ledger_name ? _fbb.CreateString(ledger_name) : 0,
      description ? _fbb.CreateString(description) : 0,
      amount,
      precision);
}

struct Asset FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ASSET_TYPE = 4,
    VT_ASSET = 6
  };
  AnyAsset asset_type() const {
    return static_cast<AnyAsset>(GetField<uint8_t>(VT_ASSET_TYPE, 0));
  }
  const void *asset() const {
    return GetPointer<const void *>(VT_ASSET);
  }
  template<typename T> const T *asset_as() const;
  const ComplexAsset *asset_as_ComplexAsset() const {
    return (asset_type() == AnyAsset_ComplexAsset)? static_cast<const ComplexAsset *>(asset()) : nullptr;
  }
  const Currency *asset_as_Currency() const {
    return (asset_type() == AnyAsset_Currency)? static_cast<const Currency *>(asset()) : nullptr;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_ASSET_TYPE) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_ASSET) &&
           VerifyAnyAsset(verifier, asset(), asset_type()) &&
           verifier.EndTable();
  }
};

template<> inline const ComplexAsset *Asset::asset_as<ComplexAsset>() const {
  return asset_as_ComplexAsset();
}

template<> inline const Currency *Asset::asset_as<Currency>() const {
  return asset_as_Currency();
}

struct AssetBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_asset_type(AnyAsset asset_type) {
    fbb_.AddElement<uint8_t>(Asset::VT_ASSET_TYPE, static_cast<uint8_t>(asset_type), 0);
  }
  void add_asset(flatbuffers::Offset<void> asset) {
    fbb_.AddOffset(Asset::VT_ASSET, asset);
  }
  AssetBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  AssetBuilder &operator=(const AssetBuilder &);
  flatbuffers::Offset<Asset> Finish() {
    const auto end = fbb_.EndTable(start_, 2);
    auto o = flatbuffers::Offset<Asset>(end);
    return o;
  }
};

inline flatbuffers::Offset<Asset> CreateAsset(
    flatbuffers::FlatBufferBuilder &_fbb,
    AnyAsset asset_type = AnyAsset_NONE,
    flatbuffers::Offset<void> asset = 0) {
  AssetBuilder builder_(_fbb);
  builder_.add_asset(asset);
  builder_.add_asset_type(asset_type);
  return builder_.Finish();
}

inline bool VerifyAnyAsset(flatbuffers::Verifier &verifier, const void *obj, AnyAsset type) {
  switch (type) {
    case AnyAsset_NONE: {
      return true;
    }
    case AnyAsset_ComplexAsset: {
      auto ptr = reinterpret_cast<const ComplexAsset *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case AnyAsset_Currency: {
      auto ptr = reinterpret_cast<const Currency *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return false;
  }
}

inline bool VerifyAnyAssetVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyAnyAsset(
        verifier,  values->Get(i), types->GetEnum<AnyAsset>(i))) {
      return false;
    }
  }
  return true;
}

}  // namespace iroha

#endif  // FLATBUFFERS_GENERATED_ASSET_IROHA_H_