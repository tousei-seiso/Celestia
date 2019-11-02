#pragma once

#include <array>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <climits>
#include "selection.h"
#include "namedb.h"
#include "astroobj.h"
#include "astrocat.h"
#include "crossindex.h"
#include "astrooctree.h"
#include "dataloader.h"
#include "star.h"
#include "deepskyobj.h"
#include "body.h"
#include "solarsys.h"

class AstroDatabase {
 public:
    typedef std::map<AstroCatalog::IndexNumber, AstroObject*> MainIndex;
    typedef std::map<int, AstroDataLoader*> LoadersMap;
    typedef std::list<Star*> StarsList;
    typedef std::list<DeepSkyObject*> DsosList;
    typedef std::list<Body*> BodiesList;
    typedef std::map<AstroCatalog::IndexNumber, SolarSystem*> SolarSystemIndex;

    enum Catalog
    {
        HenryDraper  = 0,
        Gliese       = 1,
        SAO          = 2,
        Hipparcos    = 3,
        Tycho        = 4,
        MaxBuiltinCatalog = 5
    };
    static constexpr array<const char *, AstroDatabase::MaxBuiltinCatalog> CatalogPrefix  = { "HD", "Gliese", "SAO", "HIP", "TYC" };
    AstroDatabase();
    ~AstroDatabase();
 protected:
    MainIndex m_mainIndex;
    std::map<int, AstroCatalog*> m_catalogs;
    std::map<const char *, Catalog> m_prefixCatalog;
    std::map<int, CrossIndex*> m_catxindex;
    std::map<int, CrossIndex*> m_celxindex;
    AstroNameDatabase m_nameIndex;
    LoadersMap m_loaders;
    SolarSystemIndex m_systems;
    OctreeNode m_starOctree;
    OctreeNode m_dsoOctree;
    size_t m_starNum { 0 };
    size_t m_dsoNum { 0 };
    size_t m_bodyNum { 0 };
    float m_totalDsoMag { 0 };

    AstroCatalog::IndexNumber m_autoIndex;
    static const AstroCatalog::IndexNumber AutoIndexMax = UINT_MAX - 1;
    static const AstroCatalog::IndexNumber AutoIndexMin = HipparcosAstroCatalog::MaxCatalogNumber + 1;

    AstroCatalog::IndexNumber getAutoIndex();

    void createBuiltinCatalogs();

    bool addName(const NameInfo::SharedConstPtr &);
    bool addLocalizedName(const NameInfo::SharedConstPtr &);

    friend AstroObject;
    friend NameInfo;
 public:

    AstroObject *getObject(AstroCatalog::IndexNumber) const;
    AstroObject *getObject(const Name&, bool = true, bool = true) const;
    Star *getStar(AstroCatalog::IndexNumber) const;
    DeepSkyObject *getDSO(AstroCatalog::IndexNumber) const;
    Star *getStar(const Name&, bool = true, bool = true) const;
    DeepSkyObject *getDSO(const Name&, bool = true, bool = true) const;
    size_t size() const { return m_mainIndex.size(); };

    AstroCatalog::IndexNumber nameToIndex(const Name&, bool = true, bool = true) const;
    AstroCatalog::IndexNumber starnameToIndex(const Name& name, bool = true) const;

    AstroCatalog::IndexNumber catalogNumberToIndex(int, AstroCatalog::IndexNumber) const;
    AstroCatalog::IndexNumber indexToCatalogNumber(int, AstroCatalog::IndexNumber) const;

    std::string catalogNumberToString(AstroCatalog::IndexNumber) const;
    std::string catalogNumberToString(int, AstroCatalog::IndexNumber) const;

    Name getObjectName(AstroCatalog::IndexNumber, bool = false) const;
    Name getObjectName(const AstroObject *o, bool i18n = false) const
    {
        return getObjectName(o->getIndex(), i18n);
    }

    std::vector<Name> getObjectNameList(AstroCatalog::IndexNumber, int = 128) const;
    std::vector<Name> getObjectNameList(AstroObject *o, int n = 128) const
    {
        return getObjectNameList(o->getIndex(), n);
    }

    std::string getObjectNames(AstroCatalog::IndexNumber, bool = true, int = 128) const;
    std::string getObjectNames(const AstroObject *o, int n = 128) const
    {
        return getObjectNames(o->getIndex(), n);
    }

    std::vector<Name> getCompletion(const std::string& name) const
    {
        return m_nameIndex.getCompletion(name);
    }

    bool addAstroCatalog(int, AstroCatalog*);
    bool addCatalogNumber(AstroCatalog::IndexNumber, int, AstroCatalog::IndexNumber, bool = false);
    bool addCatalogRange(AstroCatalog::IndexNumber, int, int, size_t, bool = false);

    bool addObject(AstroObject *);
    bool addStar(Star *);
    bool addDSO(DeepSkyObject *);
    bool addBody(Body *);

    bool removeObject(AstroCatalog::IndexNumber);
    bool removeObject(AstroObject *);

    bool addName(AstroCatalog::IndexNumber, const Name&);
    void addNames(AstroCatalog::IndexNumber, const std::string&);

    const NameInfo::SharedConstPtr &getNameInfo(const Name& name) const { return m_nameIndex.getNameInfo(name); }

    void removeName(const Name& name) { m_nameIndex.erase(name); }
    void removeName(const NameInfo::SharedConstPtr &);
    void removeNames(AstroCatalog::IndexNumber);
    void removeNames(AstroObject*);

    const MainIndex &getMainIndex() const
    {
        return m_mainIndex;
    }

    bool addSystem(SolarSystem*, AstroCatalog::IndexNumber);
    SolarSystem *getSystem(AstroCatalog::IndexNumber) const;
    bool removeSystem(AstroCatalog::IndexNumber);
    SolarSystemIndex &getSystems() { return m_systems; }
    const SolarSystemIndex &getSystems() const { return m_systems; }

    OctreeNode* getStarOctree() { return &m_starOctree; }
    OctreeNode* getDsoOctree() { return &m_dsoOctree; }
    const OctreeNode* getStarOctree() const { return &m_starOctree; }
    const OctreeNode* getDsoOctree() const { return &m_dsoOctree; }

    float avgDsoMag() const;

    CrossIndex *getCelestiaCrossIndex(int);
    const CrossIndex *getCelestiaCrossIndex(int) const;
    CrossIndex *getCatalogCrossIndex(int);
    const CrossIndex *getCatalogCrossIndex(int) const;
    AstroNameDatabase &getAstroNameDatabase() { return m_nameIndex; }
    size_t getStarNumber() const { return m_starNum; }
    size_t getDsoNumber() const { return m_dsoNum; }
    size_t getBodyNumber() const { return m_bodyNum; }

    void dumpStats() const;
};