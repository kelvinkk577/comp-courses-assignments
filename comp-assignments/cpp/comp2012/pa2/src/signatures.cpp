#include "signatures.h"
#include "number.h"

SigType Signature::GetType() const
{
    string name = typeid(*this).name();

    if (name == "12DSASignature")
        return DSA;
    else if (name == "16SchnorrSignature")
        return Schnorr;
    else
        throw invalid_argument("Signature Type does not exist!");
}

//Todo: Constructors/Destructors

SchnorrSignature::SchnorrSignature(const Number* s, const Number* e)
{
    this->s = s;
    this->e = e;
}

SchnorrSignature::~SchnorrSignature()
{
    cout << "Destruct SchnorrSignature..." << endl;
    delete s;
    delete e;
}

DSASignature::DSASignature(const Number* r, const Number* s)
{
    this->r = r;
    this->s = s;
}

DSASignature::~DSASignature()
{
    cout << "Destruct DSASignature..." << endl;
    delete r;
    delete s;
}

SchnorrPublicKey::~SchnorrPublicKey()
{
    cout << "Destruct SchnorrPublicKey..." << endl;
    delete y;
}

DSAPublicKey::DSAPublicKey(const Number* y)
{
    this->y = y;
}

DSAPublicKey::~DSAPublicKey()
{
    cout << "Destruct DSAPublicKey..." << endl;
    delete y;
}

SchnorrSecretKey::SchnorrSecretKey(const Number* x)
{
    this->x = x;
}

SchnorrSecretKey::~SchnorrSecretKey()
{
    cout << "Destruct SchnorrSecretKey..." << endl;
    delete x;
}

DSASecretKey::~DSASecretKey()
{
    cout << "Destruct DSASecretKey..." << endl;
    delete x;
}

// a > b, return 1
// a = b, return 0
// a < b, return -1
int compare(Number a, Number b)
{
    return (Number::NSign(Number::Sub(a, b)));
}

bool SchnorrPublicKey::Verify(const string &message, const Signature &signature) const
{
    //Todo
    const SchnorrSignature& schSig = dynamic_cast<const SchnorrSignature&>(signature); 

    if (compare(*schSig.s, Number(1)) < 0)
        return false;
    if (compare(*schSig.s, Number::Sub(*Number::Q, Number(1))) > 0)
        return false;
    if (compare(*schSig.e, Number(1)) < 0)
        return false;
    if (compare(*schSig.e, Number::Sub(*Number::Q, Number(1))) > 0)
        return false;
    
    Number gs = Number::Pow(*Number::G, *schSig.s, *Number::P);
    Number ye = Number::Pow(*y, *schSig.e, *Number::P);

    Number rv = Number::Mul_Mod(gs, ye, *Number::P);
    Number ev = Number::Mod(Number::Hash(rv, message), *Number::Q);

    if (compare(ev, *schSig.e) == 0)
        return true;

    return false;
}


bool DSAPublicKey::Verify(const string &message, const Signature &signature) const
{
    //Todo
    const DSASignature& dsaSig = dynamic_cast<const DSASignature&>(signature);
    
    if (compare(*dsaSig.r, Number(1)) < 0)
        return false;
    if (compare(*dsaSig.r, Number::Sub(*Number::Q, Number(1))) > 0)
        return false;
    if (compare(*dsaSig.s, Number(1)) < 0)
        return false;
    if (compare(*dsaSig.s, Number::Sub(*Number::Q, Number(1))) > 0)
        return false;

    Number w = Number::Inv(*dsaSig.s, *Number::Q);
    Number z = Number::Hash(message);
    Number u1 = Number::Mul_Mod(z, w, *Number::Q);
    Number u2 = Number::Mul_Mod(*dsaSig.r, w, *Number::Q);

    Number gu1 = Number::Pow(*Number::G, u1, *Number::P);
    Number yu2 = Number::Pow(*y, u2, *Number::P);
    
    Number gu1yu2 = Number::Mod(Number::Mul_Mod(gu1, yu2, *Number::P), *Number::Q);
    
    if (compare(gu1yu2, *dsaSig.r) == 0)
        return true;

    return false;
}

const Signature *SchnorrSecretKey::Sign(const string &message) const
{
    //Todo
    while (true)
    {
        Number k = Number::Rand(1, *Number::Q);
        Number r = Number::Pow(*Number::G, k, *Number::P);
        Number e = Number::Mod(Number::Hash(r, message), *Number::Q);
        Number s = Number::Mod(Number::Sub(k, Number::Mul_Mod(*(this->x), e, *Number::Q)), *Number::Q);

        if (Number::NSign(s) != 0 && Number::NSign(e) != 0)
            return new SchnorrSignature(new Number(s), new Number(e));
    }
}



const Signature *DSASecretKey::Sign(const string &message) const
{
    while (true)
    {
        Number k = Number::Rand(1, *Number::Q);
        Number r = Number::Mod(Number::Pow(*Number::G, k, *Number::P), *Number::Q);
        Number z = Number::Hash(message);
        Number zxr = Number::Mod(Number::Add(z, Number::Mul_Mod(*(this->x), r, *Number::Q)), *Number::Q);
        Number ki = Number::Inv(k, *Number::Q);
        Number s = Number::Mul_Mod(ki, zxr, *Number::Q);

        if (Number::NSign(r) > 0 && Number::NSign(s) > 0)
            return new DSASignature(new Number(r), new Number(s));
    }
}

SigPair::SigPair(const PublicKey *publicKey, const SecretKey *secretKey)
    : publicKey(publicKey), secretKey(secretKey)
{
}

SigPair::~SigPair()
{
    cout << "Destruct SigPair..."<<endl;
    delete publicKey;
    delete secretKey;
}

const SigPair *GenerateKey(SigType sigType, const string &info)
{
    if (sigType == DSA )
    {
        Number x = Number::Rand(Number(1), *Number::Q);
        Number y = Number::Pow(*Number::G, x, *Number::P);
        const Number *newX = new Number(x);
        const Number *newY = new Number(y);

        DSASecretKey *sk = new DSASecretKey(newX);
        DSAPublicKey *pk = new DSAPublicKey(newY);

        SigPair *sigPair = new SigPair(pk, sk);

        return sigPair;
    }
    else if (sigType == Schnorr )
    {
        //Todo
        Number x = Number::Rand(Number(1), *Number::Q);
        Number y = Number::Pow(*Number::G, x, *Number::P);
        const Number *newX = new Number(x);
        const Number *newY = new Number(y);

        SchnorrSecretKey *sk = new SchnorrSecretKey(newX);
        SchnorrPublicKey *pk = new SchnorrPublicKey(newY);

        SigPair *sigPair = new SigPair(pk, sk);

        return sigPair;
    }
        return nullptr;
}

vector<SigType> SigTypes()
{
    vector<SigType> s;
    s.push_back(DSA);
    s.push_back(Schnorr);
    return s;
}
