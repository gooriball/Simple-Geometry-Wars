#ifndef ENTITY_MANAGER_H
# define ENTITY_MANAGER_H

# include <vector>
# include <map>
# include <memory>
# include <algorithm>

# include "Entity.h"

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager
{
	public:
		EntityManager();;

		void	update();

		std::shared_ptr<Entity>	addEntity(const std::string& tag);
		const EntityVec&		getEntities() const;
		const EntityVec&		getEntities(const std::string& tag);
		const EntityMap&		getEntityMap() const;

	private:
		void		removeDeadEntities(EntityVec& vec);

		EntityVec	entities_;
		EntityVec	entitiesToAdd_;
		EntityMap	entityMap_;
		size_t		totalEntities_ = 0;
};

#endif