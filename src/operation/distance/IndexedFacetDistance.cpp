/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2016 Daniel Baston
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation.
 * See the COPYING file for more information.
 *
 **********************************************************************
 *
 * Last port: operation/distance/IndexedFacetDistance.java (f6187ee2 JTS-1.14)
 *
 **********************************************************************/

#include <geos/index/strtree/STRtree.h>
#include <geos/operation/distance/IndexedFacetDistance.h>

using namespace geos::geom;
using namespace geos::index::strtree;

namespace geos {
    namespace operation {
        namespace distance {
            double IndexedFacetDistance::distance(const Geometry * g1, const Geometry * g2) {
                IndexedFacetDistance ifd(g1);
                return ifd.getDistance(g2);
            }

            double IndexedFacetDistance::getDistance(const Geometry * g) const {
                struct : public ItemDistance {
                    double distance(const ItemBoundable* item1, const ItemBoundable* item2) {
                        return static_cast<const FacetSequence*>(item1->getItem())->distance(*static_cast<const FacetSequence*>(item2->getItem()));
                    }
                } itemDistance;

                std::auto_ptr<STRtree> tree2(FacetSequenceTreeBuilder::build(g));

                std::pair<const void*, const void*> obj = cachedTree->nearestNeighbour(tree2.get(), dynamic_cast<ItemDistance*>(&itemDistance));

                return static_cast<const FacetSequence*>(obj.first)->distance(*static_cast<const FacetSequence*>(obj.second));
            }
        }
    }
}