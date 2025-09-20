#ifndef ENTITY_H
# define ENTITY_H

# include "Components.h"

# include <string>
# include <tuple>

class EntityManager;

using ComponentTuple = std::tuple<
								TransformComponent,
								ShapeComponent,
								CollisionComponent,
								ScoreComponent,
								LifespanComponent,
								InputComponent
								>;

class Entity
{
	friend class EntityManager;

	public:
		bool				isActive() const { return (active_); }
		void				destroy() { active_ = false; }
		size_t				id() const { return (id_); }
		const std::string&	tag() const { return (tag_); }

		template<typename T>
		bool				hasComponent() const { return (getComponent<T>().exists_); }

		template<typename T, typename... TArgs>
		T&					addComponent(TArgs&&... mArgs)
		{
			auto& component = getComponent<T>();
			component = T(std::forward<TArgs>(mArgs)...);
			component.exists_ = true;
			
			return (component);
		}

		template<typename T>
		T&					getComponent() { return (std::get<T>(components_)); }

		template<typename T>
		const T&			getComponent() const { return (std::get<T>(components_)); }

	private:
		Entity(const size_t& id, const std::string& tag) :
			id_{id}, tag_{tag} {}

		ComponentTuple	components_;
		bool			active_ = true;
		std::string		tag_ = "default";
		size_t			id_ = 0;
};

#endif