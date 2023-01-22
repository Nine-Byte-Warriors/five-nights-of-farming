#include "stdafx.h"
#include "ProjectileManager.h"

#define INITIAL_POOL_COUNT 10

ProjectileManager::ProjectileManager()
{
	// TODO: should be passed in from Projectile JSON
	m_fLifeTime = 1.0f; 
	float fSpeed = 50.0f; 
	m_fDelay = 0.0f;
	m_fCounter = 0.0f;
	m_sProjectileImagePath = "";

	m_vecProjectilePool = std::vector<std::shared_ptr<Projectile>>();
	for (int i = 0; i < INITIAL_POOL_COUNT; i++)
		m_vecProjectilePool.push_back(std::make_shared<Projectile>(fSpeed));
	
	AddToEvent();
}

ProjectileManager::~ProjectileManager()
{
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::TargetPosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerFire, this);
}

void ProjectileManager::Initialize(const Graphics& gfx, ConstantBuffer<Matrices>& mat)
{
	if (m_sProjectileImagePath == "")
		for(std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
			pProjectile->Initialize(gfx, mat, Sprite::Type::Projectile);
	else
		for (std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
			pProjectile->Initialize(gfx, mat, m_sProjectileImagePath);
}

void ProjectileManager::InitialiseFromFile(const Graphics& gfx, ConstantBuffer<Matrices>& mat, const std::string& filename)
{
	for (std::shared_ptr<Projectile>& pProjectile : m_vecProjectilePool)
		pProjectile->Initialize(gfx, mat, filename);
}

void ProjectileManager::Update( const float dt )
{
	if (m_fCounter >= 0.0f)
	{
		m_fCounter -= dt;
		return;
	}

	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		pProjectile->Update(dt);
}

void ProjectileManager::Draw( ID3D11DeviceContext* context, XMMATRIX orthoMatrix )
{
	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		pProjectile->Draw(context, orthoMatrix);
}

void ProjectileManager::LoadProjectileData(const std::string& sFilePath)
{
	std::vector<ProjectileData::ManagerJSON> vecManagers;
	JsonLoading::LoadJson(vecManagers, sFilePath);
	
	if (vecManagers.size() < 1)
		return;
	
	SetDelay(vecManagers[0].m_fDelay);
	SetProjectileImagePath(vecManagers[0].m_sImagePath);
	SetProjectilePool(CreateProjectilePool(vecManagers[0].m_vecProjectiles));
}

std::vector<std::shared_ptr<Projectile>> ProjectileManager::CreateProjectilePool(std::vector<ProjectileData::ProjectileJSON> vecProjectileJsons)
{
	std::vector<std::shared_ptr<Projectile>> vecProjectilePool;

	for (ProjectileData::ProjectileJSON pJson : vecProjectileJsons)
	{
		std::shared_ptr<Projectile> pProjectile = std::make_shared<Projectile>(pJson.m_fSpeed, pJson.m_fLifeTime);
		pProjectile->SetDirection(Vector2f(pJson.m_fAngle));
		pProjectile->SetOffSet(Vector2f(pJson.m_fX, pJson.m_fY));
		pProjectile->SetWave(pJson.m_fAngle, pJson.m_fAmplitude, pJson.m_fFrequency);

		vecProjectilePool.push_back(std::move(pProjectile));
	}

	return vecProjectilePool;
}

void ProjectileManager::SpawnProjectile()
{
	m_fCounter = m_fDelay;

	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();

	if (pProjectile != nullptr)
		pProjectile->SpawnProjectile(m_vSpawnPosition, m_vTargetPosition, m_fLifeTime);
}

void ProjectileManager::SpawnProjectile(Vector2f vSpawnPosition, float fLifeTime)
{
	m_fCounter = m_fDelay;

	std::shared_ptr<Projectile> pProjectile = GetFreeProjectile();
	
	if(pProjectile != nullptr)
		pProjectile->SpawnProjectile(vSpawnPosition, m_fLifeTime);
}

void ProjectileManager::SpawnProjectiles(Vector2f vSpawnPosition)
{
	m_fCounter = m_fDelay;

	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		pProjectile->SpawnProjectile(vSpawnPosition, -1.0f);
}

void ProjectileManager::SpawnProjectiles(Vector2f vSpawnPosition, Vector2f vTargetPosition)
{
	m_fCounter = m_fDelay;

	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		pProjectile->SpawnProjectile(vSpawnPosition, vTargetPosition, -1.0f);
}

std::shared_ptr<Projectile> ProjectileManager::GetFreeProjectile()
{
	for (std::shared_ptr<Projectile> pProjectile : m_vecProjectilePool)
		if (!pProjectile->IsAlive())
			return pProjectile;
	
	return nullptr;
}

void ProjectileManager::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::TargetPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerFire, this);
}

void ProjectileManager::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::PlayerPosition: 
		m_vSpawnPosition = *static_cast<Vector2f*>(event->GetData());
		break;
	case EVENTID::TargetPosition: 
		m_vTargetPosition = *static_cast<Vector2f*>(event->GetData());
		break;
	case EVENTID::PlayerFire:
		SpawnProjectile();
		break;
	default:
		break;
	}
}