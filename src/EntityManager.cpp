#include "EntityManager.h"

EntityManager::EntityManager() = default;

void	EntityManager::update()
{
	for (const auto& entity : entitiesToAdd_)
	{
		entities_.push_back(entity);
	}
	entitiesToAdd_.clear();

	removeDeadEntities(entities_);

	for (auto& [tag, entityVec] : entityMap_)
	{
		removeDeadEntities(entityVec);
	}
}

std::shared_ptr<Entity>	EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity{totalEntities_++, tag});
	entitiesToAdd_.push_back(entity);

	auto [iter, success] = entityMap_.try_emplace(tag, EntityVec{});
	(iter->second).push_back(entity);

	return (entity);
}

const EntityVec&	EntityManager::getEntities() const { return (entities_); }

const EntityVec&	EntityManager::getEntities(const std::string& tag) { return (entityMap_[tag]); }

const EntityMap&	EntityManager::getEntityMap() const { return (entityMap_); }


void	EntityManager::removeDeadEntities(EntityVec& vec)
{
	auto iter = std::remove_if(vec.begin(), vec.end(),
								[](const std::shared_ptr<Entity>& entity)
								{
									return (entity->isActive() == false);
								});
	vec.erase(iter, vec.end());
}
